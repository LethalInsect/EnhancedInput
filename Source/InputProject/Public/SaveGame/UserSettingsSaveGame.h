// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "UserSettingsSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class INPUTPROJECT_API UUserSettingsSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TMap<FString, FKey> SavedKeyboardKeys;
	UPROPERTY()
	TMap<FString, FKey> SavedControllerKeys;
};
