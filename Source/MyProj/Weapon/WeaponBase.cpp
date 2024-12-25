// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/WeaponBase.h"

#include <Runtime/Engine/Classes/Components/BoxComponent.h>
#include "Runtime/UMG/Public/Components/WidgetComponent.h"
#include "Runtime/Engine/Classes/Components/SkeletalMeshComponent.h"
#include "WeaponComponent.h"
#include "Projectile.h"
#include <Runtime/Engine/Classes/Components/ArrowComponent.h>
#include <MyPlayerController.h>
#include "MyProjCharacter.h"

// Sets default values
AWeaponBase::AWeaponBase()
{
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComp"));
	SetRootComponent(MeshComp);

	PickupBox = CreateDefaultSubobject<UBoxComponent>(TEXT("PickupBox"));
	PickupBox->SetupAttachment(GetRootComponent());

	PickupWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("PickupWidget"));
	PickupWidget->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	BulletCount = MaxBulletCount;
	if (PickupWidget)
	{
		PickupWidget->SetVisibility(false);
	}
	PickupBox->OnComponentBeginOverlap.AddDynamic(this, &AWeaponBase::OnPickupBoxBeginOverlap);
	PickupBox->OnComponentEndOverlap.AddDynamic(this, &AWeaponBase::OnPickupBoxEndOverlap);
	SetWeaponCollision();
}

void AWeaponBase::SetWeaponCollision()
{
	MeshComp->SetSimulatePhysics(false);
	MeshComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	MeshComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	MeshComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);

	PickupBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	PickupBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}


void AWeaponBase::ThrowWeapon(FVector ThrowDirection, float Force)
{
	MeshComp->AddImpulse(ThrowDirection * Force);
}

void AWeaponBase::SetWeaponEquipped(bool bEquip)
{
	bEquipped = bEquip;
	if (bEquip)
	{
		MeshComp->SetSimulatePhysics(false);
		PickupBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
		PickupBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		if (PickupWidget)
		{
			PickupWidget->SetVisibility(false);
		}
	}
	else
	{
		MeshComp->SetSimulatePhysics(true);
		PickupBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
		PickupBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
}

void AWeaponBase::SetOwnerCharacter(ACharacter* NewCharacter)
{
	OwnerCharacter = NewCharacter;
}

void AWeaponBase::SetBulletCount(int32 NewCount)
{
	BulletCount = NewCount;
	if (OwnerCharacter)
	{
		AMyPlayerController* MyPlayerController = Cast<AMyPlayerController>(OwnerCharacter->GetController());
		if (MyPlayerController)
		{
			MyPlayerController->SetBulletCount(BulletCount, MaxBulletCount);
		}
	}
}

void AWeaponBase::OnPickupBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IsValid(OtherActor) && OtherActor->ActorHasTag(PlayerTag))
	{
		UWeaponComponent* WeaponComp = OtherActor->FindComponentByClass<UWeaponComponent>();
		if (IsValid(WeaponComp))
		{
			WeaponComp->SetReadyToPick(true, this);
			PickupWidget->SetVisibility(true);
		}
	}
}

void AWeaponBase::OnPickupBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (IsValid(OtherActor) && OtherActor->ActorHasTag(PlayerTag))
	{
		UWeaponComponent* WeaponComp = OtherActor->FindComponentByClass<UWeaponComponent>();
		if (IsValid(WeaponComp))
		{
			WeaponComp->SetReadyToPick(false);
			PickupWidget->SetVisibility(false);
		}
	}
}

