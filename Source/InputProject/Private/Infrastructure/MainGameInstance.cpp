// Fill out your copyright notice in the Description page of Project Settings.


#include "Infrastructure/MainGameInstance.h"

#include "Subsystems/GeneralSubsystem.h"
#include "Subsystems/SaveGameSubsystem.h"

void UMainGameInstance::Init()
{
	Super::Init();
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &UMainGameInstance::CallPostSubsystemInit);
}

void UMainGameInstance::Shutdown()
{
	Super::Shutdown();
}

USaveGameSubsystem* UMainGameInstance::GetSaveGameSubsystem()
{
	return SaveGameSubsystem;
}

UGeneralSubsystem* UMainGameInstance::GetGeneralGameInstanceSubsystem()
{
	return GeneralSubsystem;
}

void UMainGameInstance::CallPostSubsystemInit()
{
	SaveGameSubsystem = GetSubsystem<USaveGameSubsystem>();
	if (SaveGameSubsystem)
	{
		SaveGameSubsystem->PostSubsystemInitialization(this);
	}
	else
		UE_LOG(LogTemp, Error, TEXT("SaveGameSubsystem in GameInstance not initialized yet "));
	
	GeneralSubsystem = GetSubsystem<UGeneralSubsystem>();
	if (GeneralSubsystem)
	{
		GeneralSubsystem->PostSubsystemInitialization(this);
	}
	else
		UE_LOG(LogTemp, Error, TEXT("GeneralSubsystem in GameInstance not initialized yet "));
}

