// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

class UPauseWidget;
class UInputAction;
class UItemPackWidget;
class UHUDWidget;
/**
 * 
 */
UCLASS()
class MYPROJ_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AMyPlayerController();

	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category="UI | Pause")
	TSubclassOf<UPauseWidget> PauseWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI | Pause Actipn")
	UInputAction* PauseAction;

	UPROPERTY(EditDefaultsOnly, Category = "UI | ItemPack")
	TSubclassOf<UItemPackWidget> ItemPackWidgetClass;

	UPROPERTY(EditAnywhere, Category = "UI | ItemPack Action")
	UInputAction* ItemPackAction;

	UPROPERTY(EditDefaultsOnly, Category = "UI | HUD")
	TSubclassOf<UHUDWidget> HUDWidgetClass;
	TObjectPtr<UHUDWidget> HUDWidget;

	void ShowPauseWidget();
	void HidePauseWidget();
	void InitializeHUD();
	void SetBulletCount(int32 BulletCount, int32 MaxBulletCount);
	void SetGunIcon(UTexture2D* NewIcon);
	void HideBulletCount();

	// no param with UFUNCTION()
	UFUNCTION()
	void OnPausePressed();

	void ShowItemPackWidget();

	void HideItemPackWidget();
	UFUNCTION()
	void OnItemPackPressed();

	void SetHealthPercent(float NewPercent);

private:
	UPROPERTY()
	TObjectPtr<UPauseWidget> PauseWidget;
	UPROPERTY()
	TObjectPtr<UItemPackWidget> ItemPackWidget;


};
