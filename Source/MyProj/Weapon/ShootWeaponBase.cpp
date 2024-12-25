// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/ShootWeaponBase.h"
#include <Runtime/Engine/Classes/Components/BoxComponent.h>
#include "Runtime/UMG/Public/Components/WidgetComponent.h"
#include "Runtime/Engine/Classes/Components/SkeletalMeshComponent.h"
#include "WeaponComponent.h"
#include "Projectile.h"
#include <Runtime/Engine/Classes/Components/ArrowComponent.h>
#include <MyPlayerController.h>
#include "MyProjCharacter.h"
#include <Runtime/Engine/Classes/Camera/CameraComponent.h>
#include "Kismet/KismetSystemLibrary.h"

AShootWeaponBase::AShootWeaponBase()
{
	ArrowComp = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComp"));
	ArrowComp->SetupAttachment(GetRootComponent());
}

void AShootWeaponBase::Fire()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	if (Projectile && ArrowComp && BulletCount > 0)
	{
		GetWorld()->SpawnActor<AProjectile>(Projectile, ArrowComp->GetComponentLocation(), ArrowComp->GetComponentRotation(), SpawnParams);
		SetBulletCount(BulletCount - 1);
	}
}

void AShootWeaponBase::OnRightMousePressed()
{
	if (AMyProjCharacter* MyProjCharacter = Cast<AMyProjCharacter>(OwnerCharacter))
	{
		if (UCameraComponent* Camera = MyProjCharacter->GetCamera())
		{
			FAttachmentTransformRules Rule = FAttachmentTransformRules::KeepWorldTransform;

			if (USceneComponent* Root = Cast<USceneComponent>(MyProjCharacter->GetAimBoom()))
			{
				Camera->AttachToComponent(Root, Rule);
			}
			MyProjCharacter->bUseControllerRotationYaw = true;

			FLatentActionInfo LatentInfo;
			LatentInfo.CallbackTarget = this;
			LatentInfo.ExecutionFunction = FName(TEXT("OnCameraMoveComplete"));
			LatentInfo.UUID = 1;
			LatentInfo.Linkage = 0;

			UKismetSystemLibrary::MoveComponentTo(Camera, FVector(0.f, 0.f, 0.f), FRotator(0.f, 0.f, 0.f), true, true, 0.2f, false, EMoveComponentAction::Move, LatentInfo);
			
		}
	}
}

void AShootWeaponBase::OnCameraMoveComplete()
{
	if (AMyProjCharacter* MyProjCharacter = Cast<AMyProjCharacter>(OwnerCharacter))
	{
		MyProjCharacter->ShowCrossHair();
	}
}

void AShootWeaponBase::OnRightMouseReleased()
{
	if (AMyProjCharacter* MyProjCharacter = Cast<AMyProjCharacter>(OwnerCharacter))
	{
		if (UCameraComponent* Camera = MyProjCharacter->GetCamera())
		{
			FAttachmentTransformRules Rule = FAttachmentTransformRules::KeepRelativeTransform;
			if (USceneComponent* CameraBoom = Cast<USceneComponent>(MyProjCharacter->GetCameraBoom()))
			{
				Camera->AttachToComponent(CameraBoom, Rule);
			}
			MyProjCharacter->bUseControllerRotationYaw = false;

			MyProjCharacter->HideCrossHair();

			UKismetSystemLibrary::MoveComponentTo(Camera, FVector(0.f, 0.f, 0.f), FRotator(0.f, 0.f, 0.f), true, true, 0.2f, false, EMoveComponentAction::Move, FLatentActionInfo());
			
		}
	}
}
