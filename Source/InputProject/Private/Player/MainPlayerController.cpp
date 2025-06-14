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

/// Gets all mappable Keys from the InputMappingContext and sends them to the GeneralSubsystem to process further
void AMainPlayerController::InitializeKeyBinds()
{
	//If you want to add more mapping context you could just add:
	//for ( IMC for TArray<IMC>) ...same logic as further down
	
	TArray<FEnhancedActionKeyMapping> Mappings = IMCDefault->GetMappings();
	for (FEnhancedActionKeyMapping Mapping : Mappings)
	{
		//this only returns something if the input action has the mappble key settings added to it (this means static Key binds will not be picked up and can ignored)
		//you could also make a category with "NotMapple" and just not add that category to the Rebinding Section or handle it differently
		if (const UPlayerMappableKeySettings* Settings = Mapping.GetPlayerMappableKeySettings())
		{
			
			FString CurrentKeyName =  Settings->DisplayName.ToString();
			FString Category = Settings->DisplayCategory.ToString();
			if(!Mapping.Key.IsGamepadKey())
			{
				//This is a hacky way to determine it, but I trust the devs to not mess up the keybinds in the mapping context for other devices
				//Since c++ doesn't have a "IsKeyboardKey" I just pretend that all the otherkeys beside gamepad are keyboardkeys
				
				//UE_LOG(Custom_Testing, Error, TEXT("KeyBoard Key: %s"), *Mapping.Key.GetDisplayName().ToString());

				//const cast is for converting the const inputaction* to a normal inputaction*
				//the key is here the defaultkey from the inputaction.
				GeneralSubsystem->InitializeMappableKeys(false, const_cast<UInputAction*>(Mapping.Action.Get()),CurrentKeyName,Category, Mapping.Key);
			}
			else
			{
				//If it is a controller key it comes here
				
				//UE_LOG(Custom_Testing, Error, TEXT("Controller Key: %s"), *Mapping.Key.GetDisplayName().ToString());
				GeneralSubsystem->InitializeMappableKeys(true, const_cast<UInputAction*>(Mapping.Action.Get()),CurrentKeyName,Category, Mapping.Key);
			}
		}
	}

	//After every key is initialized we wait a short time and then initialize all Widgets with these keys
	// I haven't found a solution to do this without the timer,
	// so I leaved it here since this would be initialized when the game starts and not during runtime
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([this]()
	{
		GeneralSubsystem->WasMappableKeysAlreadyInitialized = true;
		GeneralSubsystem->OnMappableKeysInitialized.Broadcast();
	}), 0.3f, false);
	
}