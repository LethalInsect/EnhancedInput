// Fill out your copyright notice in the Description page of Project Settings.


#include "RunTimeRichTextDecorator.h"

void URunTimeRichTextDecorator::AddImageBrush(FName Tag, const FSlateBrush& Brush)
{
	RuntimeImageMap.Add(Tag, Brush);
}

FText URunTimeRichTextDecorator::FormatText(const FText& Template, const TArray<FText>& Args)
{
	TArray<FFormatArgumentValue> FormatArgs;
	for (const FText& Arg : Args)
	{
		FormatArgs.Add(Arg);
	}
	return FText::Format(Template, FormatArgs);
}

const FSlateBrush* URunTimeRichTextDecorator::FindImageBrush(FName Tag, bool bWarnIfMissing)
{
	const FSlateBrush* SlateBrush = RuntimeImageMap.Find(Tag);
	if (SlateBrush)
	{
		return SlateBrush;
	}
	// Fallback to base behavior (e.g., data table)
	return Super::FindImageBrush(Tag, bWarnIfMissing);
}