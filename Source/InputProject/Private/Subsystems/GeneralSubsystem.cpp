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
#include "Framework/Application/NavigationConfig.h"
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
	//this function is called from the GameInstance after every subsystem is loaded.
	
	GameInstance = MyGameInstance;
	
	//Keyboard navigation can be enabled here but for my usecase its better without
	TSharedRef<FNavigationConfig> navigation_config = MakeShared<FNavigationConfig>();
	
	navigation_config->bTabNavigation = false;
	navigation_config->bKeyNavigation = false;
	navigation_config->bAnalogNavigation = true;
	FSlateApplication::Get().SetNavigationConfig(navigation_config);
	
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

/// loads the data tables into a Map 
/// @param KeyBoardDataTable DataTable of all keyboard key icons
/// @param DualSenseDataTable DataTable of all controller key icons
void UGeneralSubsystem::InitializeKeyInfo(UDataTable* KeyBoardDataTable, UDataTable* DualSenseDataTable)
{
	//in this function we load first all icon into the maps, so we can easily access them later.
	//this happens before any key bindings are loaded.
	
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

	//here we wait for a short delay, so all icons can be safely stored. I tried it without but i didnt work without a timer
	//any solutions are welcomed here.
	
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([this]()
	{
		OnKeyInitialized.Broadcast();
		//WasKeyAlreadyInitialized is for a sticky event (when it was already triggered, logic can still be executed with the bool check).
		WasKeyAlreadyInitialized = true;
		
	}), 0.3f, false);
}

/// Load every mappable key into the maps and loads saved keys if savegame already exists.
/// @param IsController Is the mapped key a controller
/// @param InputAction  The inputaction tied to the Key
/// @param DisplayName  Display name of the inputaction
/// @param CategoryName Name of the mapped key category
/// @param DefaultKey Default key of the Inputactionslot. If savedgame exists then the saved key will be applied and if not then the default key will the mapped key
void UGeneralSubsystem::InitializeMappableKeys(bool IsController, UInputAction* InputAction, FString DisplayName,
                                               FString CategoryName, FKey DefaultKey)
{
	if (!InputAction) return;

	
	//Check if the map already has an entry for that input action and then update it with the new key
	FInputActionData* ExistingInfo = MappableInputActionData.Find(InputAction);
	FKey CurrentKey;
	//check if there is already a savegame existing if not then the defaultkey will be the currentkey.
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

/// returns Data from the given Input Action
/// @param Action Input Action from which you want the data
/// @return Data for the given Input Action
FInputActionData UGeneralSubsystem::GetInputData(UInputAction* Action)
{
	FInputActionData* ExistingInfo = MappableInputActionData.Find(Action);
	if (ExistingInfo)
		return *ExistingInfo;
	
	UE_LOG(Custom_Testing, Error, TEXT("Key was not found in GeneralSubsystem"));
	FInputActionData* Data = new FInputActionData();
	return *Data;
}

/// Saves the Key and remappes it internally to the given inputaction
/// @param InputAction Input Action mapped to key
/// @param IsController is the Key a gamepadkey
/// @param Args Key args made to remap the key to the enhanced input action system
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
		
		//Every UI is subscribing to the event and updates the given icons.
		OnKeyUpdated.Broadcast();
	}
}

/// gets and icon for the given key
/// @param Key Key to find corresponding icon
/// @return returns key icon if found, otherwise returns purple icon if nothing was found
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

///  gets the current stored MappableInputActionData Map
/// @return the entire map for every mappable icon
TMap<UInputAction*, FInputActionData> UGeneralSubsystem::GetMappableInputActionData()
{
	return MappableInputActionData;
}

/// returns icon for both inputs with a given Input Action
/// @param InputAction Input Action to search
/// @param KeyboardIcon Return icon for keyboard key
/// @param ControllerIcon Return icon for controller key
void UGeneralSubsystem::GetInputIcons(UInputAction* InputAction, UTexture2D*& KeyboardIcon, UTexture2D*& ControllerIcon)
{
	FInputActionData* ExistingInfo = MappableInputActionData.Find(InputAction);
	if (ExistingInfo)
	{
		KeyboardIcon = ExistingInfo->Icon_KeyBoard;
		ControllerIcon = ExistingInfo->Icon_Controller;
	}
}

/// Turns on or off the controller Navigation for the UI
/// @param ShouldEnable should enable navigation
void UGeneralSubsystem::EnableUINavigation(bool ShouldEnable)
{
	TSharedRef<FNavigationConfig> navigation_config = MakeShared<FNavigationConfig>();
	if(ShouldEnable)
	{
		navigation_config->bTabNavigation = false;
		navigation_config->bKeyNavigation = false;
		navigation_config->bAnalogNavigation = true;
	}
	else
	{
		navigation_config->bTabNavigation = false;
		navigation_config->bKeyNavigation = false;
		navigation_config->bAnalogNavigation = false;
	}
	FSlateApplication::Get().SetNavigationConfig(navigation_config);
}
