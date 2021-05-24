// Copyright Epic Games, Inc. All Rights Reserved.
/*
* RobotGladiatorCharacter.h
* Purpose: Declares the specific functionality of our player, inherits from Unreal's generic character class
* Dependencies: Gameframework/Character.h
* Primary Author: Ethan Heil
*/

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RobotGladiatorCharacter.generated.h"

UCLASS(config=Game)
class ARobotGladiatorCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
public:
	ARobotGladiatorCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsLockedOnEnemy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsSprinting;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BaseSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LockOnSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SprintMultiplier;

protected:

	// Called for forwards/backward input
	void MoveForward(float Value);

	// Called for side to side input
	void MoveRight(float Value);
	
	// Turns the camera left/right (with joystick)
	void TurnAtRate(float Rate);

	// Moves the camera up/down (with joystick)
	void LookUpAtRate(float Rate);

	// Start sprinting
	void Sprint();

	// Stop sprinting
	void EndSprint();

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

	// Called at the start of the current scene
	virtual void BeginPlay() override;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

