// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MainGameInstance.generated.h"

class UGeneralSubsystem;
class USaveGameSubsystem;
/**
 * 
 */
UCLASS()
class INPUTPROJECT_API UMainGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	virtual void Init() override;
	virtual void Shutdown() override;
	USaveGameSubsystem* GetSaveGameSubsystem();
	UGeneralSubsystem* GetGeneralGameInstanceSubsystem();
private:
	UPROPERTY()
	USaveGameSubsystem* SaveGameSubsystem;
	UPROPERTY()
	UGeneralSubsystem* GeneralSubsystem;
	void CallPostSubsystemInit();

};
