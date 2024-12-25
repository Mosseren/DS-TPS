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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CPP | atk动画")
	TArray<UAnimMontage*> AttackMontages;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBoxComponent* LeftHitBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBoxComponent* RightHitBox; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CPP | 左HitBox套接字")
	FName LeftHitBoxSocketName = "FX_WeaponBase_L";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CPP | 右HitBox套接字")
	FName RightHitBoxSocketName = "FX_WeaponBase_R";

	void SetUpHitBox(UBoxComponent* HitBox);

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
	UFUNCTION()
	void OnBeginOverlapPlayer(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
