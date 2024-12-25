// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.h"
#include "ShootWeaponBase.generated.h"

class UBoxComponent;
class UWidgetComponent;
class AProjectile;
class UArrowComponent;
class ACharacter;


UCLASS()
class MYPROJ_API AShootWeaponBase : public AWeaponBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AShootWeaponBase();

	UPROPERTY(EditAnywhere, BlueprintReadonly, meta = (AllowPrivateAccess = "true"))
	UArrowComponent* ArrowComp;

public:	
	virtual void Fire() override;
	virtual void OnRightMousePressed() override;
	UFUNCTION()
	void OnCameraMoveComplete();
	virtual void OnRightMouseReleased() override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadonly, Category = "CPP | 关联子弹", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AProjectile> Projectile;
};
