// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUDWidget.generated.h"

class UTextBlock;
class UProgressBar;
class UImage;

UCLASS()
class MYPROJ_API UHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(meta = (BindWidget))
	UTextBlock* BulletCount;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthBar;

	UPROPERTY(meta = (BindWidget))
	UImage* GunIcon;
public:
	void SetBulletCount(int32 Count, int32 MaxCount);
	void SetGunIcon(UTexture2D* NewTexture2D);
	void SethealthPercent(float Percent);
	void HideBulletCount();
};
