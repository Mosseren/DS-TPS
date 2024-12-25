// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponComponent.generated.h"


class AMyProjCharacter;
class AWeaponBase;

enum class EWeaponSocket : uint8
{
	Primary,
	Secondary,
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MYPROJ_API UWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWeaponComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	AWeaponBase* SpawnDefaultWeapon();
	bool AttachWeapon(AWeaponBase* WeaponToEquip, EWeaponSocket WeaponSocket);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetReadyToPick(bool bReay, AWeaponBase* PickWeapon = nullptr);

	bool IsInParryWindow();
	bool IsMelleMode();

	FORCEINLINE bool GetIsReadyToPick() { return bReayToPick; };

	void PickupWeapon();

	void DropWeapon(bool bIsPrimary);
	void SwitchWeapon();
	void Fire();
	void OnRightMousePressed();
	void OnRightMouseReleased();

private:
	UPROPERTY()
	AMyProjCharacter* OwnerCharacter;
	
	UPROPERTY(EditDefaultsOnly, Category = "Weapon", meta = (DisplayName = "默认武器"))
	TSubclassOf<AWeaponBase> DefaultWeapon;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon", meta = (DisplayName = "主射击武器插槽"))
	FName PrimaryShootWeaponSocketName = FName("PrimaryShootWeaponSocket");
	UPROPERTY(EditDefaultsOnly, Category = "Weapon", meta = (DisplayName = "主近战武器插槽"))
	FName PrimaryMelleWeaponSocketName = FName("PrimaryMelleWeaponSocket");

	UPROPERTY(EditDefaultsOnly, Category = "Weapon", meta = (DisplayName = "副武器插槽"))
	FName SecondaryWeaponSocketName = FName("SecondaryWeaponSocket");

	void SetPrimaryWeapon(AWeaponBase* NewWeapon);

private:
	UPROPERTY()
	AWeaponBase* PrimaryWeapon;

	UPROPERTY()
	AWeaponBase* SecondaryWeapon;

	void EquipWeapon(AWeaponBase* WeaponToEquip);

	bool bReayToPick = false;

	UPROPERTY()
	AWeaponBase* TempWeapon;
};
