// Fill out your copyright notice in the Description page of Project Settings.
#include "EnemyWidget.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"
#include "GameFramework/Actor.h"
#include "Blueprint/UserWidget.h"
#include <Components/CanvasPanelSlot.h>


void UEnemyWidget::UUserWidget()
{
	HealthBar = nullptr;
}

void UEnemyWidget::UpdateHealth(float Percent)
{
	HealthPercent = Percent;
	FVector2D CurHealthSize = CurHealthSlot->GetSize();
	CurHealthSize.X = Percent * MaxHealthSizeX;
	CurHealthSlot->SetSize(CurHealthSize);
}

void UEnemyWidget::NativeTick(const FGeometry& MyGeometry, float DeltaTime)
{
	if (LagPercent > HealthPercent)
	{
		LagPercent -= DeltaTime * LagSpeed;
	}
	else
	{
		LagPercent = HealthPercent;
	}
	HealthBar->SetPercent(LagPercent);
}

void UEnemyWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// »ñÈ¡¿Ø¼þ
	HealthBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("HealthBar")));
	CurHealth = Cast<UImage>(GetWidgetFromName(TEXT("CurHealth")));

	if (HealthBar && CurHealth)
	{
		HealthBar->SetPercent(1.f);

		UCanvasPanelSlot* HealthBarSlot = CastChecked<UCanvasPanelSlot>(HealthBar->Slot);
		MaxHealthSizeX = HealthBarSlot->GetSize().X;

		CurHealthSlot = CastChecked<UCanvasPanelSlot>(CurHealth->Slot);
		UpdateHealth(1.f);
	}
}

