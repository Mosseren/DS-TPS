// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemPackWidget.h"
#include "MyGameInstance.h"
#include "Components/TileView.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/SizeBox.h"

void UItemPackWidget::NativeOnInitialized()
{
	OnWeaponClicked();

	if (WeaponButton != nullptr)
	{
		WeaponButton->OnClicked.AddDynamic(this, &UItemPackWidget::OnWeaponClicked);
	}
	if (ShieldButton != nullptr)
	{
		ShieldButton->OnClicked.AddDynamic(this, &UItemPackWidget::OnShieldClicked);
	}
	if (AccessoryButton != nullptr)
	{
		AccessoryButton->OnClicked.AddDynamic(this, &UItemPackWidget::OnAccessoryClicked);
	}
	if (ConsumableButton != nullptr)
	{
		ConsumableButton->OnClicked.AddDynamic(this, &UItemPackWidget::OnConsumableClicked);
	}
	Buttons.Add(WeaponButton);
	Buttons.Add(ShieldButton);
	Buttons.Add(AccessoryButton);
	Buttons.Add(ConsumableButton);


	UWidget* ParentWidget = WeaponButton->GetParent();
	if (USizeBox* ParentSizeBox = Cast<USizeBox>(ParentWidget))
	{
		// 获取当前的宽高值
		float CurrentWidth = ParentSizeBox->GetWidthOverride();
		float CurrentHeight = ParentSizeBox->GetHeightOverride();

		OriginalButtonSize = FVector2D(CurrentWidth, CurrentHeight);
	}
	ResizeButton(WeaponButton, true);

}

void UItemPackWidget::RefreshCategoryTileView(EItemType ItemType)
{
	if (ItemTileView)
	{
		UMyGameInstance* GameInstance = Cast<UMyGameInstance>(GetGameInstance());
		if (GameInstance)
		{
			const TArray<UGameItemInfo*>& CategoryItems = GameInstance->ItemInfos[ItemType];
			ItemTileView->ClearListItems();

			// 添加武器对象到ItemTileView
			for (UObject* Item : CategoryItems)
			{
				ItemTileView->AddItem(Item);
			}
		}
	}
}

void UItemPackWidget::FocusOnButton(UButton* ButtonToFofus)
{
	for (UButton* CurButton : Buttons)
	{
		ResizeButton(CurButton, CurButton == ButtonToFofus);
	}
}

void UItemPackWidget::ResizeButton(UButton* Button, bool bIsFocused)
{
	UWidget* ParentWidget = Button->GetParent();
	if (USizeBox* ParentSizeBox = Cast<USizeBox>(ParentWidget))
	{
		if (ParentSizeBox->bOverride_HeightOverride && ParentSizeBox->bOverride_WidthOverride)
		{
			if (bIsFocused)
			{
				ParentSizeBox->SetHeightOverride(OriginalButtonSize.Y * ResizeButtonScale);
				ParentSizeBox->SetWidthOverride(OriginalButtonSize.X * ResizeButtonScale);
			}
			else
			{
				ParentSizeBox->SetHeightOverride(OriginalButtonSize.Y);
				ParentSizeBox->SetWidthOverride(OriginalButtonSize.X);
			}
		}
	}
}

void UItemPackWidget::OnWeaponClicked()
{
	RefreshCategoryTileView(EItemType::Weapon);
	FocusOnButton(WeaponButton);
}

void UItemPackWidget::OnShieldClicked()
{
	RefreshCategoryTileView(EItemType::Shield);
	FocusOnButton(ShieldButton);
}

void UItemPackWidget::OnAccessoryClicked()
{
	RefreshCategoryTileView(EItemType::Accessory);
	FocusOnButton(AccessoryButton);
}

void UItemPackWidget::OnConsumableClicked()
{
	RefreshCategoryTileView(EItemType::Consumable);
	FocusOnButton(ConsumableButton);
}

// 蓝图中绑定
void UItemPackWidget::OnItemClicked(UObject* Item)
{
	UGameItemInfo* ItemInfo = Cast<UGameItemInfo>(Item);
	if (ItemInfo)
	{
		ItemName->SetText(FText::FromName(ItemInfo->Name));
		ItemCount->SetText(FText::FromString(FString::FromInt(ItemInfo->Count)));
		ItemDescription->SetText(FText::FromName(ItemInfo->Description));
		ItemIcon->SetBrushFromTexture(ItemInfo->Texture);
	}
}
