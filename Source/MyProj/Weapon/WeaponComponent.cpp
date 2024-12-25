// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/WeaponComponent.h"
#include <MyProjCharacter.h>
#include "WeaponBase.h"
#include <MyPlayerController.h>
#include "MelleWeaponBase.h"


// Sets default values for this component's properties
UWeaponComponent::UWeaponComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UWeaponComponent::BeginPlay()
{
	Super::BeginPlay();
	OwnerCharacter = GetOwner<AMyProjCharacter>();

	AMyPlayerController* MyPlayerController = Cast<AMyPlayerController>(OwnerCharacter->GetController());
	if (IsValid(MyPlayerController))
	{
		MyPlayerController->InitializeHUD();
	}

	EquipWeapon(SpawnDefaultWeapon());
}


// Called every frame
void UWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UWeaponComponent::SetReadyToPick(bool bReay, AWeaponBase* PickWeapon)
{
	bReayToPick = bReay;
	if (PickWeapon)
	{
		TempWeapon = PickWeapon;
	}
	else
	{
		TempWeapon = nullptr;
	}
}

bool UWeaponComponent::IsInParryWindow()
{
	if (PrimaryWeapon)
	{
		if (AMelleWeaponBase* MelleWeaponBase = Cast<AMelleWeaponBase>(PrimaryWeapon))
		{
			return MelleWeaponBase->bInParryWindow;
		}
	}
	return false;
}

bool UWeaponComponent::IsMelleMode()
{
	if (PrimaryWeapon)
	{
		if (AMelleWeaponBase* MelleWeaponBase = Cast<AMelleWeaponBase>(PrimaryWeapon))
		{
			true;
		}
	}
	return false;
}

void UWeaponComponent::PickupWeapon()
{
	if (IsValid(TempWeapon))
	{
		EquipWeapon(TempWeapon);
	}
}

void UWeaponComponent::DropWeapon(bool bIsPrimary)
{
	if (!IsValid(OwnerCharacter))
	{
		return;
	}
	AWeaponBase* DroppedWeapon = nullptr;
	FVector ThrowDirection;
	float Force;

	if (bIsPrimary && IsValid(PrimaryWeapon)) {
		DroppedWeapon = PrimaryWeapon;
		SetPrimaryWeapon(nullptr);
		// 计算丢弃武器的方向
		ThrowDirection = OwnerCharacter->GetActorRotation().Vector();
		Force = 1000.f;
		// 副武器转为主武器
		if (IsValid(SecondaryWeapon))
		{
			if (AttachWeapon(SecondaryWeapon, EWeaponSocket::Primary))
			{
				SetPrimaryWeapon(SecondaryWeapon);
				SecondaryWeapon = nullptr;
			}
		}
	}
	if (!bIsPrimary && IsValid(SecondaryWeapon))
	{
		DroppedWeapon = SecondaryWeapon;
		SecondaryWeapon = nullptr;

		FRotator PlayerRotation = OwnerCharacter->GetActorRotation();
		FRotator ThrowRotator = FRotator(0, -PlayerRotation.Yaw, 0);
		ThrowDirection = ThrowRotator.Vector();
		Force = 700.f;
	}
	if (IsValid(DroppedWeapon)) {
		FDetachmentTransformRules DetachmentTransformRules(EDetachmentRule::KeepWorld, true);
		DroppedWeapon->GetRootComponent()->DetachFromComponent(DetachmentTransformRules);
		DroppedWeapon->SetWeaponEquipped(false);
		DroppedWeapon->ThrowWeapon(ThrowDirection, Force);
	}
}

void UWeaponComponent::SwitchWeapon()
{
	if (IsValid(PrimaryWeapon) && IsValid(SecondaryWeapon))
	{
		AttachWeapon(PrimaryWeapon, EWeaponSocket::Secondary);
		AttachWeapon(SecondaryWeapon, EWeaponSocket::Primary);
		AWeaponBase* Temp = SecondaryWeapon;
		SecondaryWeapon = PrimaryWeapon;
		SetPrimaryWeapon(Temp);
	}
}

void UWeaponComponent::SetPrimaryWeapon(AWeaponBase* NewWeapon)
{
	PrimaryWeapon = NewWeapon;
	AMyPlayerController* MyPlayerController = Cast<AMyPlayerController>(OwnerCharacter->GetController());
	if (IsValid(MyPlayerController))
	{
		if (IsValid(PrimaryWeapon))
		{
			MyPlayerController->SetGunIcon(PrimaryWeapon->GunIcon);
			MyPlayerController->SetBulletCount(PrimaryWeapon->BulletCount, PrimaryWeapon->MaxBulletCount);
			if (IsValid(PrimaryWeapon->AnimLayer))
			{
				OwnerCharacter->GetMesh()->LinkAnimClassLayers(PrimaryWeapon->AnimLayer);
			}
		}
		else
		{
			MyPlayerController->SetGunIcon(nullptr);
			MyPlayerController->HideBulletCount();
		}
	}
}

void UWeaponComponent::EquipWeapon(AWeaponBase* WeaponToEquip)
{
	if (!IsValid(OwnerCharacter) || !IsValid(WeaponToEquip))
	{
		return;
	}
	if (!IsValid(PrimaryWeapon))
	{
		if (AttachWeapon(WeaponToEquip, EWeaponSocket::Primary))
		{
			SetPrimaryWeapon(WeaponToEquip);
		}
	}
	else if (!IsValid(SecondaryWeapon))
	{
		if (AttachWeapon(WeaponToEquip, EWeaponSocket::Secondary))
		{
			SecondaryWeapon = WeaponToEquip;
		}
	}
	else
	{
		DropWeapon(false);
		if (AttachWeapon(WeaponToEquip, EWeaponSocket::Secondary))
		{
			SecondaryWeapon = WeaponToEquip;
		}
	}
}

AWeaponBase* UWeaponComponent::SpawnDefaultWeapon()
{
	if (!IsValid(OwnerCharacter))
	{
		return nullptr;
	}
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	if (DefaultWeapon)
	{
		return GetWorld()->SpawnActor<AWeaponBase>(DefaultWeapon, FTransform(), SpawnParams);
	}
	return nullptr;
}

bool UWeaponComponent::AttachWeapon(AWeaponBase* WeaponToEquip, EWeaponSocket WeaponSocket)
{
	if (!IsValid(OwnerCharacter) || !IsValid(WeaponToEquip))
	{
		return false;
	}

	FName SocketName = "";
	if (WeaponSocket == EWeaponSocket::Primary)
	{
		if (Cast<AMelleWeaponBase>(WeaponToEquip))
		{
			SocketName = PrimaryMelleWeaponSocketName;
		}
		else
		{
			SocketName = PrimaryShootWeaponSocketName;
		}
	}
	else
	{
		SocketName = SecondaryWeaponSocketName;
	}

	const USkeletalMeshSocket* Socket = OwnerCharacter->GetMesh()->GetSocketByName(SocketName);
	if (Socket)
	{
		if (USkeletalMeshComponent* WeaponMesh = WeaponToEquip->GetMesh())
		{
			WeaponToEquip->SetWeaponEquipped(true);
			WeaponToEquip->AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, SocketName);
			WeaponToEquip->SetOwnerCharacter(OwnerCharacter);
			return true;
		}
	}
	return false;
}

void UWeaponComponent::Fire()
{
	if (IsValid(PrimaryWeapon))
	{
		PrimaryWeapon->Fire();
	}
}

void UWeaponComponent::OnRightMousePressed()
{
	if (IsValid(PrimaryWeapon))
	{
		PrimaryWeapon->OnRightMousePressed();
	}
}

void UWeaponComponent::OnRightMouseReleased()
{
	if (IsValid(PrimaryWeapon))
	{
		PrimaryWeapon->OnRightMouseReleased();
	}
}

