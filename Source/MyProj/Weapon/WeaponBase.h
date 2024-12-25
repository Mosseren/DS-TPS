// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"

class UBoxComponent;
class UWidgetComponent;
class AProjectile;
class UArrowComponent;
class ACharacter;

UCLASS()
class MYPROJ_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AWeaponBase();

public:
	UPROPERTY(EditAnywhere, BlueprintReadonly, Category = "CPP | 子弹数目", meta = (AllowPrivateAccess = "true"))
	int32 MaxBulletCount;
	int32 BulletCount;
	int32 FireConsume = 1;

	UPROPERTY(EditAnywhere, BlueprintReadonly, Category = "CPP | Icon", meta = (AllowPrivateAccess = "true"))
	UTexture2D* GunIcon;

	UPROPERTY(EditAnywhere, BlueprintReadonly, Category = "CPP | 动画层", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UAnimInstance> AnimLayer;

	FORCEINLINE USkeletalMeshComponent* GetMesh() { return MeshComp; };
	void ThrowWeapon(FVector ThrowDirection, float Force);
	void SetWeaponEquipped(bool bEquip);
	void SetOwnerCharacter(ACharacter* NewCharacter);
	virtual void Fire() {};
	virtual void OnRightMousePressed() {};
	virtual void OnRightMouseReleased() {};


protected:
	UPROPERTY()
	ACharacter* OwnerCharacter;

	bool bEquipped = false;

	UPROPERTY(EditAnywhere, BlueprintReadonly, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* MeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadonly, meta = (AllowPrivateAccess = "true"))
	UBoxComponent* PickupBox;

	UPROPERTY(EditAnywhere, BlueprintReadonly, Category = "CPP | 提示UI", meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* PickupWidget;

	UPROPERTY(EditAnywhere, BlueprintReadonly, meta = (AllowPrivateAccess = "true"))
	FName PlayerTag = FName("Player");

	virtual void BeginPlay() override;

	void SetWeaponCollision();
	void SetBulletCount(int32 NewCount);

	UFUNCTION()
	void OnPickupBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnPickupBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
