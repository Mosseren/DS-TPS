// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Widget.h"
#include "EnemyWidget.generated.h"

class UProgressBar;
class UImage;
class UCanvasPanelSlot;

UCLASS()
class MYPROJ_API UEnemyWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UProgressBar* HealthBar;
	UImage* CurHealth;

	void UUserWidget();

	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void UpdateHealth(float Percent);

protected:
	float HealthPercent;
	float LagPercent;
	float LagSpeed = 0.3f;
	UCanvasPanelSlot* CurHealthSlot;
	double MaxHealthSizeX;
};
