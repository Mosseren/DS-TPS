// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "MyProjCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class UCrossHairWidget;
class UWeaponComponent;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);
UCLASS(config=Game)
class AMyProjCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* AimBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* PickupAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SwitchAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* DropAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* FireAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CPP | 受伤动画蒙太奇", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* HurtMontage;

public:
	AMyProjCharacter();
	FORCEINLINE USpringArmComponent* GetAimBoom() { return AimBoom;};
	FORCEINLINE UCameraComponent* GetCamera() { return FollowCamera;};

protected:
	/** Called for movement input */
	void Move(const FInputActionValue& Value);
	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	void PickupWeapon(const FInputActionValue& Value);
	void SwitchWeapon(const FInputActionValue& Value);
	void DropWeapon(const FInputActionValue& Value);
	void Fire(const FInputActionValue& Value);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay();

	UFUNCTION(BlueprintCallable)
	int32 CalRollIdx(float Angle);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "准星UI", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UCrossHairWidget> CrossHairClass; //暂时由蓝图创建设置
	TObjectPtr<UCrossHairWidget> CrossHair;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "武器组件", meta = (AllowPrivateAccess = "true"))
	UWeaponComponent* WeaponComp;

public:
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class USpringArmComponent* GetAimBoom() const { return AimBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	void ShowCrossHair();
	void HideCrossHair();
	UFUNCTION()
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "生命值", meta = (AllowPrivateAccess = "true"))
	float MaxHealth;

	float Health;

	void SetHealth(float NewHealth);

	UFUNCTION( BlueprintCallable, meta = (AllowPrivateAccess = "true"))
	void OnRightMousePressed();
	UFUNCTION(BlueprintCallable, meta = (AllowPrivateAccess = "true"))
	void OnRightMouseReleased();
};

