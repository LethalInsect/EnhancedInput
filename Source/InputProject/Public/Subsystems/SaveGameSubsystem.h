// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SaveGameSubsystem.generated.h"

class UUserSettingsSaveGame;
class UMainGameInstance;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSaveGameLoaded, bool, IsNewSaveGame);
/**
 * 
 */
UCLASS()
class INPUTPROJECT_API USaveGameSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	FString SettingSaveGameSlotName = "User_Settings";
	void SaveSettingsToSlot();
	UPROPERTY()
	UMainGameInstance* GameInstance;
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	void PostSubsystemInitialization(UMainGameInstance* MyGameInstance);
	UPROPERTY(BlueprintAssignable, Category = Miscellaneous)
	FOnSaveGameLoaded OnSaveGameLoaded;
	UPROPERTY()
	UUserSettingsSaveGame* UserSettingsSaveGame;
	UPROPERTY(BlueprintReadOnly)
	bool DoUserSettingsExist = false;

	FKey GetCurrentKey(FString ActionName, bool IsController);
	void SaveKey(FString ActionName, bool IsController, FKey Key);
	
};
