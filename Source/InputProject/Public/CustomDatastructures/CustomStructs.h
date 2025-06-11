#pragma once

#include "CoreMinimal.h"
#include "CustomStructs.generated.h"
class UInputAction;

USTRUCT(BlueprintType)
struct FInputActionData
{
	GENERATED_BODY()
	UPROPERTY(BlueprintReadWrite)
	FString ActionName;
	UPROPERTY(BlueprintReadWrite)
	FString Category;
	UPROPERTY(BlueprintReadWrite)
	FKey DefaultKey_Keyboard;
	UPROPERTY(BlueprintReadWrite)
	FKey CurrentKey_KeyBoard;
	UPROPERTY(BlueprintReadWrite)
	UTexture2D* Icon_KeyBoard;
	
	UPROPERTY(BlueprintReadWrite)
	FKey DefaultKey_Controller;
	UPROPERTY(BlueprintReadWrite)
	FKey CurrentKey_Controller;
	UPROPERTY(BlueprintReadWrite)
	UTexture2D* Icon_Controller;
	
	UPROPERTY(BlueprintReadWrite)
	UInputAction* InputAction;
};

USTRUCT(BlueprintType)
struct FKeyInfo
{
	GENERATED_BODY()
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FKey Key;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UTexture2D* Icon;
	
};
USTRUCT(BlueprintType)
struct FKeyboardKeys: public FTableRowBase
{
	GENERATED_BODY()
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FKeyInfo> Keys;
};
