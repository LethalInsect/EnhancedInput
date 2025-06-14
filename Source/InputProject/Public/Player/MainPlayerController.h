// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainPlayerController.generated.h"

class UEnhancedInputLocalPlayerSubsystem;
class UInputMappingContext;
class UGeneralSubsystem;
/**
 * 
 */
UCLASS()
class INPUTPROJECT_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()
	virtual void BeginPlay() override;
public:
	UPROPERTY()
	UGeneralSubsystem* GeneralSubsystem;
	UPROPERTY()
	UEnhancedInputLocalPlayerSubsystem* InputSubsystem;

	//you can add more InputMappingContexts here if you need to,
	//or remake this to an InputmappingContext Array and just loop over all of them
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputMappingContext* IMCDefault;

	//Gets all mappable Keys and sends them to the GeneralSubsystem
	UFUNCTION()
	void InitializeKeyBinds();
};
