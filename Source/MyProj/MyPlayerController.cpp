// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"
#include "PauseWidget.h"
#include "ItemPackWidget.h"
#include "InputActionValue.h"
#include "GameFramework/PlayerController.h"
#include "MyGameInstance.h"
#include "Blueprint/UserWidget.h"
#include "UI/HUDWidget.h"

AMyPlayerController::AMyPlayerController()
{
}

void AMyPlayerController::BeginPlay()
{
	UMyGameInstance* GameInstance = Cast<UMyGameInstance>(GetGameInstance());
	if (GameInstance)
	{
		GameInstance->LoadData();
	}
}

void AMyPlayerController::ShowPauseWidget()
{
	// 延迟创建
	if (PauseWidget == nullptr && PauseWidgetClass != nullptr)
	{
		PauseWidget = CreateWidget<UPauseWidget>(this, PauseWidgetClass);
		PauseWidget->AddToViewport();
	}

	if (PauseWidget != nullptr)
	{
		// 暂停
		SetPause(true);
		SetInputMode(FInputModeUIOnly());
		bShowMouseCursor = true;

		PauseWidget->SetVisibility(ESlateVisibility::Visible);
	}
	PauseWidget->PlayAnimationForward(PauseWidget->PopUp);
}

void AMyPlayerController::HidePauseWidget()
{
	if (PauseWidget != nullptr)
	{
		PauseWidget->SetVisibility(ESlateVisibility::Collapsed);
		// 恢复
		SetPause(false);
		SetInputMode(FInputModeGameOnly());
		bShowMouseCursor = false;
	}
}

void AMyPlayerController::InitializeHUD()
{
	// 创建HUD
	if (HUDWidget == nullptr && HUDWidgetClass != nullptr)
	{
		HUDWidget = CreateWidget<UHUDWidget>(this, HUDWidgetClass);
		if (HUDWidget != nullptr)
		{
			HUDWidget->AddToViewport();
		}
	}
}

void AMyPlayerController::SetBulletCount(int32 BulletCount, int32 MaxBulletCount)
{
	if (HUDWidget)
	{
		HUDWidget->SetBulletCount(BulletCount, MaxBulletCount);
	}
}

void AMyPlayerController::SetGunIcon(UTexture2D* NewIcon)
{
	if (HUDWidget)
	{
		HUDWidget->SetGunIcon(NewIcon);
	}
}

void AMyPlayerController::HideBulletCount()
{
	if (HUDWidget)
	{
		HUDWidget->HideBulletCount();
	}
}

void AMyPlayerController::OnPausePressed()
{
	UE_LOG(LogTemp, Warning, TEXT("!!!PAUSE"));
	if (PauseWidget)
	{
		if (PauseWidget->IsVisible())
		{
			HidePauseWidget();
		}
		else
		{
			ShowPauseWidget();
		}
	}
	else
	{
		ShowPauseWidget();
	}
}

void AMyPlayerController::ShowItemPackWidget()
{
	// 延迟创建
	if (ItemPackWidget == nullptr && ItemPackWidgetClass != nullptr)
	{
		ItemPackWidget = CreateWidget<UItemPackWidget>(this, ItemPackWidgetClass);
		ItemPackWidget->AddToViewport();
	}

	if (ItemPackWidget != nullptr)
	{
		// 暂停
		SetPause(true);
		SetInputMode(FInputModeUIOnly());
		bShowMouseCursor = true;

		ItemPackWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void AMyPlayerController::HideItemPackWidget()
{
	if (ItemPackWidget != nullptr)
	{
		ItemPackWidget->SetVisibility(ESlateVisibility::Collapsed);
		// 恢复
		SetPause(false);
		SetInputMode(FInputModeGameOnly());
		bShowMouseCursor = false;
	}
}

void AMyPlayerController::OnItemPackPressed()
{
	UE_LOG(LogTemp, Warning, TEXT("!!!ItemPack"));
	if (ItemPackWidget)
	{
		if (ItemPackWidget->IsVisible())
		{
			HideItemPackWidget();
		}
		else
		{
			ShowItemPackWidget();
		}
	}
	else
	{
		ShowItemPackWidget();
	}
}

void AMyPlayerController::SetHealthPercent(float NewPercent)
{
	if (HUDWidget)
	{
		HUDWidget->SethealthPercent(NewPercent);
	}
}


