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
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputMappingContext* IMCDefault;
	UFUNCTION()
	void InitializeKeyBinds();
};
