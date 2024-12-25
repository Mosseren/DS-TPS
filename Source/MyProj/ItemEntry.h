// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "ItemEntry.generated.h"

class UImage;
class UTextBlock;
class UGameItemInfo;
/**
 * 
 */
UCLASS()
class MYPROJ_API UItemEntry : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> ItemIconImage;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemCountText;
	
	TObjectPtr<UGameItemInfo> ItemInfo;

	UFUNCTION()
	virtual	void NativeOnListItemObjectSet(UObject* ListItemObject) override;
};
