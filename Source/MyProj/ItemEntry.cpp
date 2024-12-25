// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemEntry.h"
#include "Components/Image.h"
#include "GameItemInfo.h"
#include "Components/TextBlock.h"
#include "Styling/SlateBrush.h"
#include "MyGameInstance.h"
#include "ItemPackWidget.h"

void UItemEntry::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	if (ListItemObject)
	{
		ItemInfo = Cast<UGameItemInfo>(ListItemObject);
	}
	if (ItemInfo)
	{
		if (ItemIconImage && ItemInfo->Texture)
		{
			ItemIconImage->SetBrushFromTexture(ItemInfo->Texture);
		}

		UE_LOG(LogTemp, Warning, TEXT("entry- Item count: %d"), ItemInfo->Count);
		ItemCountText->SetText(FText::FromString(FString::FromInt(ItemInfo->Count)));
	}
}


