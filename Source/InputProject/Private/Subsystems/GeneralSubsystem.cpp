// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystems/GeneralSubsystem.h"

#include "EnhancedInputSubsystems.h"
#include "Infrastructure/MainGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "SaveGame/UserSettingsSaveGame.h"
#include "Subsystems/SaveGameSubsystem.h"
#include "UserSettings/EnhancedInputUserSettings.h"
#include "CustomDatastructures/CustomStructs.h"
#include "PlayerMappableInputConfig.h"
DEFINE_LOG_CATEGORY(Custom_Testing)

void UGeneralSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	LastCheckPointLocation = FVector().Zero();
}

void UGeneralSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

void UGeneralSubsystem::PostSubsystemInitialization(UMainGameInstance* MyGameInstance)
{
	GameInstance = MyGameInstance;
	InitializeSubsystems();
}
void UGeneralSubsystem::InitializeSubsystems()
{
	if(!SGSubsystem)
	{
		SGSubsystem = GameInstance->GetSaveGameSubsystem();
		if(!SGSubsystem)
			UE_LOG(LogTemp, Error, TEXT("Save game subsystem was not found in GeneralGameinstanceSubsystem "));
	}
}

void UGeneralSubsystem::InputDeviceChanged(bool IsController)
{
	OnInputDeviceChanged.Broadcast(IsController);
}

void UGeneralSubsystem::InitializeKeyInfo(UDataTable* KeyBoardDataTable, UDataTable* DualSenseDataTable)
{
	static const FString ContextString(TEXT("My Data Table Context"));
	if(FKeyboardKeys* Row = KeyBoardDataTable->FindRow<FKeyboardKeys>("DefaultRowName", ContextString))
	{
		KeyboardKeys = Row->Keys;
		for (auto Keyinfo : KeyboardKeys)
		{
			if(!Keyinfo.Key.IsValid())
			{
				FailTex = Keyinfo.Icon;
				continue;
			}
			KeyboardKeysIconMap.Add(Keyinfo.Key, Keyinfo.Icon);
		}
	}
	if(FKeyboardKeys* Row = DualSenseDataTable->FindRow<FKeyboardKeys>("DefaultRowName", ContextString))
	{
		DualSense5Keys = Row->Keys;
		
		for (auto Keyinfo : DualSense5Keys)
		{
			if(!Keyinfo.Key.IsValid())
			{
				FailTex = Keyinfo.Icon;
				continue;
			}
			DualSense5KeysIconMap.Add(Keyinfo.Key, Keyinfo.Icon);
		}
	}
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([this]()
	{
		OnKeyInitialized.Broadcast();
		WasKeyAlreadyInitialized = true;
	}), 0.3f, false);
}

void UGeneralSubsystem::InitializeMappableKeys(bool IsController, UInputAction* InputAction, FString DisplayName,
	FString CategoryName, FKey DefaultKey)
{
	if (!InputAction) return;

	
	
	FInputActionData* ExistingInfo = MappableInputActionData.Find(InputAction);
	FKey CurrentKey;
	if(SGSubsystem->DoUserSettingsExist)
	{
		CurrentKey = SGSubsystem->GetCurrentKey(DisplayName, IsController);
		FGameplayTagContainer Container;
		FMapPlayerKeyArgs Args;
		Args.MappingName = FName(*DisplayName);
		Args.Slot = IsController ? EPlayerMappableKeySlot::First : EPlayerMappableKeySlot::Second;
		Args.NewKey = CurrentKey;
		UserSettings->MapPlayerKey(Args,Container);
	}
	else
	{
		CurrentKey = DefaultKey;
	}

	
		if (ExistingInfo)
		{
			// Update existing entry
			ExistingInfo->ActionName = DisplayName;
			ExistingInfo->Category = CategoryName;
			ExistingInfo->InputAction = InputAction;
			
			if(IsController)
			{
				ExistingInfo->DefaultKey_Controller = DefaultKey;
				ExistingInfo->CurrentKey_Controller = CurrentKey;
				ExistingInfo->Icon_Controller = GetIcon(DefaultKey);
			}
			else
			{
				ExistingInfo->DefaultKey_Keyboard = DefaultKey;
				ExistingInfo->CurrentKey_KeyBoard = CurrentKey;
				ExistingInfo->Icon_KeyBoard = GetIcon(DefaultKey);
			}
			SGSubsystem->SaveKey(DisplayName, IsController, CurrentKey);
		}
		else
		{
			// Create new entry
			FInputActionData NewInfo;
			NewInfo.ActionName = DisplayName;
			NewInfo.Category = CategoryName;
			NewInfo.InputAction = InputAction;
			if(IsController)
			{
				NewInfo.DefaultKey_Controller = DefaultKey;
				NewInfo.CurrentKey_Controller = CurrentKey;
				NewInfo.Icon_Controller = GetIcon(CurrentKey);
			}
			else
			{
				NewInfo.DefaultKey_Keyboard = DefaultKey;
				NewInfo.CurrentKey_KeyBoard = CurrentKey;
				NewInfo.Icon_KeyBoard = GetIcon(CurrentKey);
			}
	
			SGSubsystem->SaveKey(DisplayName, IsController, CurrentKey);
			MappableInputActionData.Add(InputAction, NewInfo);
		}
}

FInputActionData UGeneralSubsystem::GetInputData(UInputAction* Action)
{
	FInputActionData* ExistingInfo = MappableInputActionData.Find(Action);
	if (ExistingInfo)
		return *ExistingInfo;
	
	UE_LOG(Custom_Testing, Error, TEXT("Key was not found in GeneralSubsystem"));
	FInputActionData* Data = new FInputActionData();
	return *Data;
}

void UGeneralSubsystem::SaveKey(UInputAction* InputAction, bool IsController, FMapPlayerKeyArgs Args)
{
	FKey NewKey = Args.NewKey;
	if (FInputActionData* ActionData = MappableInputActionData.Find(InputAction))
	{
		
		if(IsController)
		{
			ActionData->CurrentKey_Controller = NewKey;
			ActionData->Icon_Controller = GetIcon(NewKey);
		}
		else
		{
			ActionData->CurrentKey_KeyBoard = NewKey;
			ActionData->Icon_KeyBoard = GetIcon(NewKey);
		}
		SGSubsystem->SaveKey(ActionData->ActionName, IsController, NewKey);
		
		FGameplayTagContainer FailureReason;
		UserSettings->MapPlayerKey(Args,FailureReason);
		OnKeyUpdated.Broadcast();
	}
}

UTexture2D* UGeneralSubsystem::GetIcon(FKey Key)
{
	if(!Key.IsValid())
	{
		UE_LOG(Custom_Testing, Error, TEXT("Key was not found in GetIcon"));
		return FailTex;
	}
	UE_LOG(Custom_Testing, Log, TEXT("key: %s"), *Key.GetDisplayName().ToString());
	UTexture2D* Icon = FailTex;
	if(Key.IsGamepadKey())
	{
		if(UTexture2D** Val = DualSense5KeysIconMap.Find(Key))
		{
			Icon = *Val;
		}
	}
	else if(Key.IsMouseButton() || !Key.IsGamepadKey())
	{
		if(UTexture2D** Val = KeyboardKeysIconMap.Find(Key))
		{
			Icon = *Val;
		}
	}
	else
	{
		UE_LOG(Custom_Testing, Error, TEXT("Key was not found in GeneralSubsystem"));
	}
	return Icon;
}

TMap<UInputAction*, FInputActionData> UGeneralSubsystem::GetMappableInputActionData()
{
	return MappableInputActionData;
}

void UGeneralSubsystem::GetInputIcons(UInputAction* InputAction, UTexture2D*& KeyboardIcon, UTexture2D*& ControllerIcon)
{
	FInputActionData* ExistingInfo = MappableInputActionData.Find(InputAction);
	if (ExistingInfo)
	{
		KeyboardIcon = ExistingInfo->Icon_KeyBoard;
		ControllerIcon = ExistingInfo->Icon_Controller;
	}
}
