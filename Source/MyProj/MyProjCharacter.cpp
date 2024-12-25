// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyProjCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "MyPlayerController.h"
#include "Weapon/WeaponComponent.h"
#include "UI/CrossHairWidget.h"
#include <Weapon/MelleWeaponBase.h>
#include "EnemyCharacter.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AMyProjCharacter

AMyProjCharacter::AMyProjCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	AimBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("AimBoom"));
	// 如果吸附到根，则会有一定程度的反向旋转
	AimBoom->SetupAttachment(CameraBoom);
	AimBoom->bUsePawnControlRotation = false;

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	WeaponComp = CreateDefaultSubobject<UWeaponComponent>(TEXT("WeaponComp"));

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void AMyProjCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	SetHealth(MaxHealth);

	if (!CrossHair && IsValid(CrossHairClass))
	{
		CrossHair = CreateWidget<UCrossHairWidget>(GetWorld(), CrossHairClass);
		CrossHair->AddToViewport();
		CrossHair->SetVisibility(ESlateVisibility::Collapsed);
	}
}

int32 AMyProjCharacter::CalRollIdx(float Angle)
{
	if (Angle >= -45.f && Angle < 45.f)
	{
		return 0;
	}
	else if (Angle >= 45.f && Angle < 135.f)
	{
		return 1;
	}
	else if (Angle >= -135.f && Angle < -45.f)
	{
		return 3;
	}
	else
	{
		return 2;
	}
}

void AMyProjCharacter::ShowCrossHair()
{
	if (CrossHair)
	{

		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("准星显示"));
		CrossHair->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("准星无"));

	}
}

void AMyProjCharacter::HideCrossHair()
{
	if (CrossHair)
	{
		CrossHair->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void AMyProjCharacter::SetHealth(float NewHealth)
{
	Health = NewHealth;
	AMyPlayerController* MyPlayerController = Cast<AMyPlayerController>(GetController());
	if (MyPlayerController)
	{
		MyPlayerController->SetHealthPercent(Health / MaxHealth);
	}
}

float AMyProjCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	
	if (WeaponComp && WeaponComp->IsInParryWindow())
	{
		if (AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(DamageCauser))
		{
			EnemyCharacter->SetEnemyState(EEnemyState::KnockOut);
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("被击退"));
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("找不到enemy"));
		}
	}
	else
	{
		if (HurtMontage) // && WeaponComp->IsMelleMode())
		{
			GetMesh()->GetAnimInstance()->Montage_Play(HurtMontage);

		}
		SetHealth(Health - DamageAmount);
	}
	return Health;
}

void AMyProjCharacter::OnRightMousePressed()
{
	if (WeaponComp)
	{
		WeaponComp->OnRightMousePressed();
	}
}

void AMyProjCharacter::OnRightMouseReleased()
{
	if (WeaponComp)
	{
		WeaponComp->OnRightMouseReleased();
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void AMyProjCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMyProjCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMyProjCharacter::Look);

		// 暂停
		AMyPlayerController* MyPlayerController = Cast<AMyPlayerController>(GetController());
		if (MyPlayerController && MyPlayerController->PauseAction)
		{
			UE_LOG(LogTemp, Warning, TEXT("BIND PAUSE PRESSED"));
			EnhancedInputComponent->BindAction(MyPlayerController->PauseAction, ETriggerEvent::Triggered, MyPlayerController, &AMyPlayerController::OnPausePressed);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("controller is null"));
		}
		// 背包
		if (MyPlayerController && MyPlayerController->ItemPackAction)
		{
			UE_LOG(LogTemp, Warning, TEXT("BIND ItemPack PRESSED"));
			EnhancedInputComponent->BindAction(MyPlayerController->ItemPackAction, ETriggerEvent::Triggered, MyPlayerController, &AMyPlayerController::OnItemPackPressed);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("controller is null"));
		}
		// 拾取
		EnhancedInputComponent->BindAction(PickupAction, ETriggerEvent::Triggered, this, &AMyProjCharacter::PickupWeapon);

		EnhancedInputComponent->BindAction(SwitchAction, ETriggerEvent::Triggered, this, &AMyProjCharacter::SwitchWeapon);
		EnhancedInputComponent->BindAction(DropAction, ETriggerEvent::Triggered, this, &AMyProjCharacter::DropWeapon);
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &AMyProjCharacter::Fire);

	}
}

void AMyProjCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AMyProjCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AMyProjCharacter::PickupWeapon(const FInputActionValue& Value)
{
	if (IsValid(WeaponComp) && WeaponComp->GetIsReadyToPick())
	{
		WeaponComp->PickupWeapon();
	}
}

void AMyProjCharacter::SwitchWeapon(const FInputActionValue& Value)
{
	if (IsValid(WeaponComp))
	{
		WeaponComp->SwitchWeapon();
	}
}

void AMyProjCharacter::DropWeapon(const FInputActionValue& Value)
{
	if (IsValid(WeaponComp))
	{
		WeaponComp->DropWeapon(true);
	}
}

void AMyProjCharacter::Fire(const FInputActionValue& Value)
{
	if (IsValid(WeaponComp))
	{
		WeaponComp->Fire();
		if (CrossHair)
		{
			CrossHair->PlayAnimationForward(CrossHair->FireAnim);
		}
	}

}
