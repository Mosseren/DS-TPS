// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"
#include <Animation/StartHitBoxNotify.h>
#include <Animation/EndHitBoxNotify.h>
#include <Runtime/Engine/Classes/Components/BoxComponent.h>
#include <Runtime/Engine/Classes/Engine/DamageEvents.h>
#include "MyProjCharacter.h"

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	LeftHitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("LeftHitBox"));
	LeftHitBox->SetupAttachment(GetMesh(), LeftHitBoxSocketName);
	SetUpHitBox(LeftHitBox);
	
	RightHitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("RightHitBox"));
	RightHitBox->SetupAttachment(GetMesh(), RightHitBoxSocketName);
	SetUpHitBox(RightHitBox);
}

void AEnemyCharacter::SetUpHitBox(UBoxComponent* HitBox)
{
	if (HitBox)
	{
		HitBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		HitBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
		HitBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		HitBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	}
}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	EnemyState = EEnemyState::Patrol;

	if (AttackMontages.Num() != 0)
	{
		for (UAnimMontage* AttackMontage : AttackMontages)
		{
			const TArray<FAnimNotifyEvent> NotifyEvents = AttackMontage->Notifies;
			for (FAnimNotifyEvent NotifyEvent : NotifyEvents)
			{
				if (UStartHitBoxNotify* StartHitBoxNotify = Cast<UStartHitBoxNotify>(NotifyEvent.Notify))
				{
					StartHitBoxNotify->OnNotified.AddUObject(this, &AEnemyCharacter::OnStartHitBox);
				}
				else if (UEndHitBoxNotify* EndHitBoxNotify = Cast<UEndHitBoxNotify>(NotifyEvent.Notify))
				{
					EndHitBoxNotify->OnNotified.AddUObject(this, &AEnemyCharacter::OnEndHitBox);
				}
			}
		}
	}

	if (LeftHitBox)
	{
		LeftHitBox->OnComponentBeginOverlap.AddDynamic(this, &AEnemyCharacter::OnBeginOverlapPlayer);
	}
	if (RightHitBox)
	{
		RightHitBox->OnComponentBeginOverlap.AddDynamic(this, &AEnemyCharacter::OnBeginOverlapPlayer);
	}
}

// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemyCharacter::OnStartHitBox()
{
	if (LeftHitBox)
	{
		LeftHitBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
	if (RightHitBox)
	{
		RightHitBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
}

void AEnemyCharacter::OnEndHitBox()
{
	if (LeftHitBox)
	{
		LeftHitBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	if (RightHitBox)
	{
		RightHitBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AEnemyCharacter::OnBeginOverlapPlayer(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IsValid(OtherActor) && OtherActor->ActorHasTag(TEXT("Player")))
	{
		if (AMyProjCharacter* MyProjCharacter = Cast<AMyProjCharacter>(OtherActor))
		{
			MyProjCharacter->TakeDamage(3.f, FDamageEvent(), nullptr, this);
		}
	}
}


