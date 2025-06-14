// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "CustomDatastructures/CustomStructs.h"
#include "GeneralSubsystem.generated.h"

struct FMapPlayerKeyArgs;
// struct FKeyInfo;
// struct FInputActionData;
class UEnhancedInputUserSettings;
class UUserSettingsSaveGame;
class UInputMappingContext;
class UInputAction;
class UMainGameInstance;
class USaveGameSubsystem;
DECLARE_LOG_CATEGORY_EXTERN(Custom_Testing, Log, All);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnKeyInitialized);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMappableKeysInitialized);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnKeyUpdated);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInputDeviceChanged, bool, IsController);
/**
 * 
 */
UCLASS()
class INPUTPROJECT_API UGeneralSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	
	FVector LastCheckPointLocation;
	UPROPERTY()
	USaveGameSubsystem* SGSubsystem;
	UPROPERTY()
	UMainGameInstance* GameInstance;
	UPROPERTY()
	UTexture2D* FailTex;
	UPROPERTY()
	TArray<FKeyInfo> KeyboardKeys;
	UPROPERTY()
	TMap<FKey, UTexture2D*> KeyboardKeysIconMap;
	UPROPERTY()
	TArray<FKeyInfo> DualSense5Keys;
	UPROPERTY()
	TMap<FKey, UTexture2D*> DualSense5KeysIconMap;
	UPROPERTY()
	TMap<UInputAction*, FInputActionData> MappableInputActionData;
	
public:
	UPROPERTY()
	UEnhancedInputUserSettings* UserSettings;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputMappingContext* IMCDefault;
	
	void PostSubsystemInitialization(UMainGameInstance* MyGameInstance);
	void InitializeSubsystems();

	UFUNCTION(BlueprintCallable)
	void InputDeviceChanged(bool IsController);
	UPROPERTY(BlueprintAssignable)
	FOnInputDeviceChanged OnInputDeviceChanged;
	UPROPERTY(BlueprintAssignable)
	FOnKeyUpdated OnKeyUpdated;
	
	
	UFUNCTION(BlueprintCallable)
	void InitializeKeyInfo(UDataTable* KeyBoardDataTable, UDataTable* DualSenseDataTable);
	UFUNCTION(BlueprintCallable)
	void InitializeMappableKeys(bool IsController, UInputAction* InputAction, FString DisplayName, FString CategoryName, FKey DefaultKey);
	UFUNCTION(BlueprintCallable)
	FInputActionData GetInputData(UInputAction* Action);
	UFUNCTION(BlueprintCallable)
	void SaveKey(UInputAction* InputAction, bool IsController, FMapPlayerKeyArgs Args);
	UFUNCTION(BlueprintCallable)
	UTexture2D* GetIcon(FKey Key);
	UFUNCTION(BlueprintPure)
	TMap<UInputAction*, FInputActionData> GetMappableInputActionData();
	UFUNCTION(BlueprintCallable)
	void GetInputIcons(UInputAction* InputAction, UTexture2D*& KeyboardIcon, UTexture2D*& ControllerIcon);
	UFUNCTION(BlueprintCallable)
	void EnableUINavigation(bool ShouldEnable);
	UPROPERTY(BlueprintAssignable)
	FOnKeyInitialized OnKeyInitialized;
	UPROPERTY(BlueprintReadOnly)
	bool WasKeyAlreadyInitialized = false;

	//is called from player, after every key exists
	UPROPERTY(BlueprintAssignable)
	FOnMappableKeysInitialized OnMappableKeysInitialized;
	UPROPERTY(BlueprintReadOnly)
	bool WasMappableKeysAlreadyInitialized = false;

};
