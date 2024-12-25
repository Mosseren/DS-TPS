// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/MelleWeaponBase.h"
#include "GameFramework/Character.h"
#include "Animation/StartHitBoxNotify.h"
#include "Components/BoxComponent.h"
#include <Animation/EndHitBoxNotify.h>
#include <Animation/EndComboNotify.h>
#include <Animation/StartParryWindowNotify.h>
#include <Animation/EndParryWindowNotify.h>
#include <EnemyCharacter.h>
#include <Runtime/Engine/Classes/Engine/DamageEvents.h>

AMelleWeaponBase::AMelleWeaponBase()
{
	PrimaryActorTick.bCanEverTick = true;

	HitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("HitBox"));
	HitBox->SetupAttachment(GetRootComponent());
	HitBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	HitBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	HitBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore); 
	HitBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
}

void AMelleWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	// 绑定动画通知的响应函数
	if (AttackMontage)
	{
		NumAttackSections = AttackMontage->CompositeSections.Num();
	}
	else
	{
		NumAttackSections = 0;
	}

	if (AttackMontage)
	{
		const TArray<FAnimNotifyEvent> NotifyEvents = AttackMontage->Notifies;
		for (FAnimNotifyEvent NotifyEvent : NotifyEvents)
		{
			if (UStartHitBoxNotify* StartHitBoxNotify = Cast<UStartHitBoxNotify>(NotifyEvent.Notify))
			{
				StartHitBoxNotify->OnNotified.AddUObject(this, &AMelleWeaponBase::OnStartHitBox);
			}
			else if (UEndHitBoxNotify* EndHitBoxNotify = Cast<UEndHitBoxNotify>(NotifyEvent.Notify))
			{
				EndHitBoxNotify->OnNotified.AddUObject(this, &AMelleWeaponBase::OnEndHitBox);
			}
			else if (UEndComboNotify* EndComboNotify = Cast<UEndComboNotify>(NotifyEvent.Notify))
			{
				EndComboNotify->OnNotified.AddUObject(this, &AMelleWeaponBase::OnEndCombo);
			}
		}
	}

	if (ParryMontage)
	{
		const TArray<FAnimNotifyEvent> NotifyEvents = ParryMontage->Notifies;
		for (FAnimNotifyEvent NotifyEvent : NotifyEvents)
		{
			if (UStartParryWindowNotify* StartParryWindowNotify = Cast<UStartParryWindowNotify>(NotifyEvent.Notify))
			{
				StartParryWindowNotify->OnNotified.AddUObject(this, &AMelleWeaponBase::OnStartParryWindow);
			}
			if (UEndParryWindowNotify* EndParryWindowNotify = Cast<UEndParryWindowNotify>(NotifyEvent.Notify))
			{
				EndParryWindowNotify->OnNotified.AddUObject(this, &AMelleWeaponBase::OnEndParryWindow);
			}
		}
	}

	HitBox->OnComponentBeginOverlap.AddDynamic(this, &AMelleWeaponBase::OnBeginOverlapEnemy);
}

void AMelleWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsRotatingToControllerYaw)
	{
		FRotator CurrentRotation = OwnerCharacter->GetActorRotation();
		FRotator TargetRotation = OwnerCharacter->GetControlRotation();
		TargetRotation.Pitch = 0.0f;
		TargetRotation.Roll = 0.0f;

		FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, RotationSpeed);
		OwnerCharacter->SetActorRotation(NewRotation);

		if (NewRotation.Equals(TargetRotation, 1.0f))
		{
			bIsRotatingToControllerYaw = false;
		}
	}
}

void AMelleWeaponBase::Fire()
{

	if (AttackMontage)
	{
		UAnimInstance* AnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance();
		if (AnimInstance && NumAttackSections != 0)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("%d"), AttackIdx));
			if (bShouldHandleInput)
			{
				bShouldHandleInput = false;
				bIsRotatingToControllerYaw = true;
				//bInterruptedByAttack = true;
				AnimInstance->Montage_Play(AttackMontage);
				AnimInstance->Montage_JumpToSection(FName(FString::FromInt(AttackIdx)), AttackMontage);

				// 绑定攻击动画的中断响应函数
				FOnMontageEnded BlendEndDelegate;
				BlendEndDelegate.BindUObject(this, &AMelleWeaponBase::OnAttackBlendEnd);
				AnimInstance->Montage_SetEndDelegate(BlendEndDelegate, AttackMontage);
			}
		}
	}
}

void AMelleWeaponBase::OnRightMousePressed()
{
	if (ParryMontage)
	{
		UAnimInstance* AnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			if (bShouldHandleInput)
			{
				AnimInstance->Montage_Play(ParryMontage);

				FOnMontageEnded BlendEndDelegate;
				BlendEndDelegate.BindUObject(this, &AMelleWeaponBase::OnParryBlendEnd);
				AnimInstance->Montage_SetEndDelegate(BlendEndDelegate, ParryMontage);
			}
		}
	}
}

void AMelleWeaponBase::OnAttackBlendEnd(UAnimMontage* animMontage, bool bInterrupted)
{
	if (!bInterrupted)
	{
		return;
	}
	if (UAnimInstance* AnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance())
	{
		// 由攻击打断则消耗标志位
		/*if (bInterruptedByAttack)
		{
			bInterruptedByAttack = false;
		}
		else
		{
			OnEndHitBox();
			OnEndCombo();
		}*/
		if (!AnimInstance->Montage_IsPlaying(AttackMontage))
		{
			OnEndHitBox();
			OnEndCombo();
		}
	}
}

void AMelleWeaponBase::OnStartHitBox()
{
	bIsRotatingToControllerYaw = false;
	if (HitBox)
	{
		HitBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
}

void AMelleWeaponBase::OnEndHitBox()
{
	if (HitBox)
	{
		HitBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	if (NumAttackSections != 0)
	{
		bShouldHandleInput = true;
		AttackIdx = (AttackIdx + 1) % NumAttackSections;
	}
}

void AMelleWeaponBase::OnEndCombo()
{
	AttackIdx = 0;
}


void AMelleWeaponBase::OnStartParryWindow()
{
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("弹反开始"));
	bInParryWindow = true;
}

void AMelleWeaponBase::OnEndParryWindow()
{
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("弹反结束"));
	bInParryWindow = false;
}

void AMelleWeaponBase::OnParryBlendEnd(UAnimMontage* animMontage, bool bInterrupted)
{
	bInParryWindow = false;
}

void AMelleWeaponBase::OnBeginOverlapEnemy(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IsValid(OtherActor) && OtherActor->ActorHasTag(TEXT("Enemy")))
	{
		if (AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(OtherActor))
		{
			EnemyCharacter->TakeDamage(10.f, FDamageEvent(), nullptr, this);
		}
	}
}
