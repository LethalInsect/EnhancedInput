// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystems/SaveGameSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "SaveGame/UserSettingsSaveGame.h"
#include "Subsystems/GeneralSubsystem.h"

void USaveGameSubsystem::SaveSettingsToSlot()
{
	UGameplayStatics::SaveGameToSlot(UserSettingsSaveGame, SettingSaveGameSlotName, 0);
}
void USaveGameSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	//  USER SETTINGS
	if(UGameplayStatics::DoesSaveGameExist(SettingSaveGameSlotName, 0))
	{
		USaveGame* SG = UGameplayStatics::LoadGameFromSlot(SettingSaveGameSlotName, 0);
		UserSettingsSaveGame = Cast<UUserSettingsSaveGame>(SG);
		DoUserSettingsExist = true;
	}
	else
	{
		USaveGame* SG = UGameplayStatics::CreateSaveGameObject(UUserSettingsSaveGame::StaticClass());
		UserSettingsSaveGame = Cast<UUserSettingsSaveGame>(SG);
		DoUserSettingsExist = false;
		SaveSettingsToSlot();
	}
}

void USaveGameSubsystem::PostSubsystemInitialization(UMainGameInstance* MyGameInstance)
{
	GameInstance = MyGameInstance;
}
FKey USaveGameSubsystem::GetCurrentKey(FString ActionName, bool IsController)
{
	FKey* Key;
	if(IsController)
	{
		Key = UserSettingsSaveGame->SavedControllerKeys.Find(ActionName);
		return *Key;
	}
	else
	{
		Key = UserSettingsSaveGame->SavedKeyboardKeys.Find(ActionName);
		return *Key;
	}
}

void USaveGameSubsystem::SaveKey(FString ActionName, bool IsController, FKey Key)
{
	UE_LOG(Custom_Testing, Log, TEXT("Action Name: %s , Key: %s  IS BEING SAVED"), *ActionName, *Key.GetDisplayName().ToString())

	//Key gets saved to the Map
	if(IsController)
	{
		UserSettingsSaveGame->SavedControllerKeys.Add(ActionName, Key);
	}
	else
	{
		UserSettingsSaveGame->SavedKeyboardKeys.Add(ActionName, Key);
	}
	SaveSettingsToSlot();
}
