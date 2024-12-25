// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUDWidget.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"

void UHUDWidget::SetBulletCount(int32 Count, int32 MaxCount)
{
	if (BulletCount)
	{
		BulletCount->SetVisibility(ESlateVisibility::HitTestInvisible);
		FString BulletText = FString::Printf(TEXT("%d / %d"), Count, MaxCount);
		BulletCount->SetText(FText::FromString(BulletText));
	}
}

void UHUDWidget::SetGunIcon(UTexture2D* NewTexture2D)
{
	if (GunIcon)
	{
		if (IsValid(NewTexture2D))
		{
			GunIcon->SetVisibility(ESlateVisibility::HitTestInvisible);
			GunIcon->SetBrushFromTexture(NewTexture2D);
		}
		else
		{
			GunIcon->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void UHUDWidget::SethealthPercent(float Percent)
{
	if (HealthBar)
	{
		HealthBar->SetPercent(Percent);
	}
}

void UHUDWidget::HideBulletCount()
{
	if (BulletCount)
	{
		BulletCount->SetVisibility(ESlateVisibility::Collapsed);
	}
}
