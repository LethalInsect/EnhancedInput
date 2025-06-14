// Fill out your copyright notice in the Description page of Project Settings.


#include "RunTimeRichTextDecorator.h"

/// 
/// @param Tag name of the image
/// @param Brush brush with the image
void URunTimeRichTextDecorator::AddImageBrush(FName Tag, const FSlateBrush& Brush)
{
	RuntimeImageMap.Add(Tag, Brush);
}
/// Adds the signature of images to text so, that the rich text can render them.
/// @param Template Original Text to Format
/// @param Args Names of images to add
/// @return Formatted Text
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