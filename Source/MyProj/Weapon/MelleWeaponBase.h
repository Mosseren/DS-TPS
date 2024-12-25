// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/WeaponBase.h"
#include "MelleWeaponBase.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJ_API AMelleWeaponBase : public AWeaponBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CPP | 攻击动画蒙太奇", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* AttackMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CPP | 弹反动画蒙太奇", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* ParryMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CPP | HitBox", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* HitBox;
	
	bool bShouldHandleInput = true;
	bool bInterruptedByAttack = false;
	bool bIsRotatingToControllerYaw = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CPP | 攻击前摇旋转速度", meta = (AllowPrivateAccess = "true"))
	float RotationSpeed = 5.f;
	int32 AttackIdx = 0;
	int32 NumAttackSections;

public:
	AMelleWeaponBase();
	bool bInParryWindow = false;
	virtual void Fire() override;
	virtual void OnRightMousePressed() override;
	
	UFUNCTION()
	void OnStartHitBox();

	UFUNCTION()
	void OnEndHitBox();

	UFUNCTION()
	void OnEndCombo();

	UFUNCTION()
	void OnAttackBlendEnd(UAnimMontage* animMontage, bool bInterrupted);
	
	UFUNCTION()
	void OnStartParryWindow();

	UFUNCTION()
	void OnEndParryWindow();

	UFUNCTION()
	void OnParryBlendEnd(UAnimMontage* animMontage, bool bInterrupted);

	UFUNCTION()
	void OnBeginOverlapEnemy(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	
	
};
