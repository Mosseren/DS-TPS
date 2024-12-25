// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"
#include <Animation/StartHitBoxNotify.h>
#include <Animation/EndHitBoxNotify.h>

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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
	//UBoxComponent HitBox
}

void AEnemyCharacter::OnEndHitBox()
{
}


