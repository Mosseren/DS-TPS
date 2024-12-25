// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameItemInfo.generated.h"

UENUM(BlueprintType)
enum class EItemType : uint8
{
	Weapon,
	Armor,
	Shield,
	Consumable,
	Accessory,
};

USTRUCT(BlueprintType)
struct MYPROJ_API FMyItemInfo : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 ID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FName Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	bool IsStackable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 Count;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	UTexture2D* Texture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	EItemType ItemType;
};
/**
 * 
 */
UCLASS()
class MYPROJ_API UGameItemInfo : public UObject
{
	GENERATED_BODY()

public:

	UGameItemInfo& operator=(const FMyItemInfo& ItemInfoStruct);


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 ID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FName Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	bool IsStackable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 Count;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	UTexture2D* Texture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	EItemType ItemType;

};
