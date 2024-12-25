// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "Components/SphereComponent.h"
#include "Engine/Engine.h"
#include <GameFramework/ProjectileMovementComponent.h>
#include "GameFramework/Character.h"
#include "GameFramework/DamageType.h"
#include "GameFramework/Actor.h" 
#include "Engine/DamageEvents.h"


// Sets default values
AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	Caster = nullptr;

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("USphereComponent"));
	SphereComp->InitSphereRadius(1.f);
	SphereComp->SetNotifyRigidBodyCollision(true);
	SphereComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	SphereComp->SetCollisionResponseToAllChannels(ECR_Block);
	SphereComp->OnComponentHit.AddDynamic(this, &AProjectile::OnHitFunc);
	RootComponent = SphereComp;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("UStaticMeshComponent"));
	StaticMesh->SetupAttachment(RootComponent);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = SphereComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	InitialLifeSpan = 3.f;
}

void AProjectile::OnHitFunc(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && OtherActor->ActorHasTag(FName("Enemy")))
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("has tag"));
		ACharacter* Enemy = Cast<ACharacter>(OtherActor);
		if (Enemy)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("transferred"));
			float DamageAmount = 10.0f;
			FDamageEvent MyDamageEvent;
			AActor* DamageCauser = this;
			Enemy->TakeDamage(DamageAmount, MyDamageEvent, nullptr, DamageCauser);
		}
		Destroy();
	}
}

void AProjectile::SetCaster(ACharacter* Character)
{
	Caster = Character;
}


