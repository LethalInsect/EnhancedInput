// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/RichTextBlockImageDecorator.h"
#include "RunTimeRichTextDecorator.generated.h"

/**
 * 
 */
UCLASS()
class INPUTPROJECT_API URunTimeRichTextDecorator : public URichTextBlockImageDecorator
{
	GENERATED_BODY()
public:
	// Your dynamic data: key = tag name, value = brush
	UPROPERTY()
	TMap<FName, FSlateBrush> RuntimeImageMap;

	// Add at runtime
	UFUNCTION(BlueprintCallable)
	void AddImageBrush(FName Tag, const FSlateBrush& Brush);

	UFUNCTION(BlueprintCallable)
	FText FormatText(const FText& Template, const TArray<FText>& Args);
protected:
	virtual const FSlateBrush* FindImageBrush(FName Tag, bool bWarnIfMissing) override;
};
