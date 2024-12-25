// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameItemInfo.h"
#include "ItemPackWidget.generated.h"

class UButton;
class UTileView;
class UInputAction;
class UTextBlock;
class UImage;
/**
 * 
 */
UCLASS()
class MYPROJ_API UItemPackWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> WeaponButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ShieldButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> AccessoryButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ConsumableButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTileView> ItemTileView;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemName;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemCount;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemDescription;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> ItemIcon;

protected:
	TArray<UButton*> Buttons;
	FVector2D OriginalButtonSize;
	float ResizeButtonScale = 1.2f;

	virtual void NativeOnInitialized() override;

	void RefreshCategoryTileView(EItemType ItemType);

	void FocusOnButton(UButton* ButtonToFofus);
	void ResizeButton(UButton* Button, bool bIsFocused);

	UFUNCTION()
	void OnWeaponClicked();

	UFUNCTION()
	void OnShieldClicked();

	UFUNCTION()
	void OnAccessoryClicked();

	UFUNCTION()
	void OnConsumableClicked();

	UFUNCTION(BlueprintCallable)
	void OnItemClicked(UObject* Item);
	
};
