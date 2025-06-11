// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/MainPlayerController.h"
#include "Subsystems/GeneralSubsystem.h"

#include "EnhancedActionKeyMapping.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputSubsystemInterface.h"
#include "InputMappingContext.h"
#include "PlayerMappableKeySettings.h"
#include "UserSettings/EnhancedInputUserSettings.h"


void AMainPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	GeneralSubsystem = GetGameInstance()->GetSubsystem<UGeneralSubsystem>();
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<
			UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		InputSubsystem = Subsystem;
		GeneralSubsystem->IMCDefault = IMCDefault;
		InputSubsystem->AddMappingContext(IMCDefault, 0);
		UEnhancedInputUserSettings* Settings = Subsystem->GetUserSettings();
		Settings->RegisterInputMappingContext(IMCDefault);
		GeneralSubsystem->UserSettings = Settings;
	}
	
	if(GeneralSubsystem->WasKeyAlreadyInitialized)
		InitializeKeyBinds();
	else
		GeneralSubsystem->OnKeyInitialized.AddDynamic(this, &AMainPlayerController::InitializeKeyBinds);
}

void AMainPlayerController::InitializeKeyBinds()
{
	
	TArray<FEnhancedActionKeyMapping> Mappings = IMCDefault->GetMappings();
	for (FEnhancedActionKeyMapping Mapping : Mappings)
	{
		
		if (const UPlayerMappableKeySettings* Settings = Mapping.GetPlayerMappableKeySettings())
		{
			
			FString CurrentKeyName =  Settings->DisplayName.ToString();
			FString Category = Settings->DisplayCategory.ToString();
			if(!Mapping.Key.IsGamepadKey())
			{
				//UE_LOG(Custom_Testing, Error, TEXT("KeyBoard Key: %s"), *Mapping.Key.GetDisplayName().ToString());
				GeneralSubsystem->InitializeMappableKeys(false, const_cast<UInputAction*>(Mapping.Action.Get()),CurrentKeyName,Category, Mapping.Key);
			}
			else
			{
				//UE_LOG(Custom_Testing, Error, TEXT("Controller Key: %s"), *Mapping.Key.GetDisplayName().ToString());
				GeneralSubsystem->InitializeMappableKeys(true, const_cast<UInputAction*>(Mapping.Action.Get()),CurrentKeyName,Category, Mapping.Key);
			}
		}
	}
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([this]()
	{
		GeneralSubsystem->WasMappableKeysAlreadyInitialized = true;
		GeneralSubsystem->OnMappableKeysInitialized.Broadcast();
	}), 0.3f, false);
	
}