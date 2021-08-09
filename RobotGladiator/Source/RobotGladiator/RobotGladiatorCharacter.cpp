// Copyright Epic Games, Inc. All Rights Reserved.
/*
* RobotGladiatorCharacter.cpp
* Purpose: Defines the specific functionality of our player
* Dependencies: See includes
* Primary Author: Ethan Heil
*/

#include "RobotGladiatorCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"

/*
* ARobotGladiatorCharacter
* Default Constructor for the player character
*/
ARobotGladiatorCharacter::ARobotGladiatorCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 90.f;
	BaseLookUpRate = 90.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;
	BaseSpeed = 600.0f;
	LockOnSpeed = 400.0f;
	SprintMultiplier = 1.5f;

	// Set default player stats
	BaseAttackSpeed = 8.0f;
	SwordAttackSpeed = BaseAttackSpeed;
	AxeAttackSpeed = BaseAttackSpeed;
	MaxAttackSpeed = 20.0f;
	mMaxHealth = 100.0f;
	mHealth = mMaxHealth;
	BaseDamage = 10.0f;
	SwordDamageMultiplier = 1.0f;
	AxeDamageMultiplier = 1.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	IsLockedOnEnemy = false;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

void ARobotGladiatorCharacter::SetBaseDamage(float damage, float modifier /*= 0.0f*/)
{
	float bonus = BaseDamage * modifier;
	BaseDamage += bonus;
}

void ARobotGladiatorCharacter::SetBaseAttackSpeed(float attackSpeed, float modifier /*= 0.0f*/)
{
	float bonus = BaseAttackSpeed * modifier;
	BaseAttackSpeed += bonus;
}

void ARobotGladiatorCharacter::SetBaseMovementSpeed(float speed, float modifier /*= 0.0f*/)
{
	float bonus = GetCharacterMovement()->MaxWalkSpeed * modifier;
	GetCharacterMovement()->MaxWalkSpeed += bonus;
}

void ARobotGladiatorCharacter::SetBaseRegen(float regenRate, float modifier /*= 0.0f*/)
{
	float bonus = BaseHealthRegen * modifier;
	BaseHealthRegen += bonus;
}

/*
* SetupPlayerInputComponent
* Set up and bind player inputs
*	- Param PlayerInputComponent: The input component for this character
*/
void ARobotGladiatorCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);

	// Set up action bindings
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ARobotGladiatorCharacter::Sprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ARobotGladiatorCharacter::EndSprint);


	// Set up movement bindings
	PlayerInputComponent->BindAxis("MoveForward", this, &ARobotGladiatorCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ARobotGladiatorCharacter::MoveRight);
	
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("TurnRate", this, &ARobotGladiatorCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ARobotGladiatorCharacter::LookUpAtRate);
}

/*
* BeginPlay
* Called at the start of a scene, used for initialization
*/
void ARobotGladiatorCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Set base walk speed
	GetCharacterMovement()->MaxWalkSpeed = BaseSpeed;
}

/*
* TurnAtRate
* Called via input to turn at a given rate, used for analog input (i.e. a joystick).
*	-Param Rate: This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
*/
void ARobotGladiatorCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

/*
* LookUpAtRate
* Called via input to turn look up/down at a given rate, used for analog input (i.e. a joystick).
*	-Param Rate: This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
*/
void ARobotGladiatorCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

/*
* Sprint
* Switches the character from running to sprinting
*/
void ARobotGladiatorCharacter::Sprint()
{
	// Check if locked on to enemy
	if (!IsLockedOnEnemy)
	{
		// Enable sprinting
		IsSprinting = true;
		GetCharacterMovement()->MaxWalkSpeed = BaseSpeed * SprintMultiplier;
	}
}

/*
* EndSprint
* Switches the character from sprinting to running
*/
void ARobotGladiatorCharacter::EndSprint()
{
	// Disable sprinting and reset movement speed
	IsSprinting = false;
	GetCharacterMovement()->MaxWalkSpeed = BaseSpeed;
}

/*
* MoveForward
* Moves the player forward/backwards based on input value
*	Param Value: Normalized (0-1) input value from the player
*/
void ARobotGladiatorCharacter::MoveForward(float Value)
{
	// Validate player controller
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		FRotator Rotation;
		// find out which way is forward
		if (IsLockedOnEnemy)
		{
			// Use follow camera rotation
			Rotation = FollowCamera->GetComponentRotation();
			// set player char rotation
			SetActorRotation(FRotator(0, Rotation.Yaw, 0), ETeleportType::None);
			// Adjust player speed
			GetCharacterMovement()->MaxWalkSpeed = LockOnSpeed;
		}
		else
		{
			// Use controller rotation
			Rotation = Controller->GetControlRotation();
		}
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

/*
* MoveRight
* Moves the player right/left based on input value
*	Param Value: Normalized (0-1) input value from the player
*/
void ARobotGladiatorCharacter::MoveRight(float Value)
{
	// Validate player controller
	if ( (Controller != nullptr) && (Value != 0.0f) )
	{
		FRotator Rotation;

		// find out which way is right
		if (IsLockedOnEnemy)
		{
			// Use follow camera rotation
			Rotation = FollowCamera->GetComponentRotation();
			// set player char rotation
			SetActorRotation(FRotator(0, Rotation.Yaw, 0), ETeleportType::None);
			// Adjust player speed
			GetCharacterMovement()->MaxWalkSpeed = LockOnSpeed;
		}
		else
		{
			// Use controller rotation
			Rotation = Controller->GetControlRotation();
		}
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}
