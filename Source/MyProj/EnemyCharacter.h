// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyCharacter.generated.h"

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	Patrol,
	Chase,
	Combat,
	Hurt,
	Dead,
	Hide,
	KnockOut,
};

class UAnimMontage;
class UBoxComponent;

UCLASS()
class MYPROJ_API AEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

protected:
	// Sets default values for this character's properties
	AEnemyCharacter();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EEnemyState EnemyState = EEnemyState::Patrol;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CPP | 被击退动画")
	UAnimMontage* KnockOutMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CPP | atk1动画")
	TArray<UAnimMontage*> AttackMontages;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CPP | HitBox", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* HitBox;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	void SetEnemyState(EEnemyState NewEnemyState) { EnemyState = NewEnemyState;};

	UFUNCTION(BlueprintCallable)
	EEnemyState GetEnemyState() {return EnemyState;};

	UFUNCTION()
	void OnStartHitBox();

	UFUNCTION()
	void OnEndHitBox();
};
