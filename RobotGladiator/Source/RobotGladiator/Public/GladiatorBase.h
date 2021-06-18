// Fill out your copyright notice in the Description page of Project Settings.

/**
 * @file GladiatorBase.h
 * @brief the base c++ class to handle internal logic for gladiators
 * @dependencies BaseUnit.h
 *
 * @author Chris Foster
 * @credits 
 **/

#pragma once

#include "CoreMinimal.h"
#include "BaseUnit.h"
#include "GladiatorBase.generated.h"

/**
GladiatorBase.h
Purpose: base class for the gladiator boss
Dependencies: APawn.h/cpp  
Primary Author: Christopher Foster
References: <For the love of god>

 */
UCLASS()
class ROBOTGLADIATOR_API AGladiatorBase : public ABaseUnit
{
	GENERATED_BODY()


	/** @brief Get the closes player to the gladiator
	*	@param {TArray<AActor*>} Array - array of actors to choose from -
	*   @return {AActor*} - the closest player to the gladiator
	*/
	AActor* GetClosestPlayer(TArray<AActor*> Array);


protected:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	

public:

	AGladiatorBase();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	AActor* mpTarget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> mClasstoFind;

	UFUNCTION(BlueprintImplementableEvent)
	void MeleeAttack();

	UFUNCTION(BlueprintImplementableEvent)
	void RangedAttack();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float mStoppingDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float mMeleeRange;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float mDistanceToTarget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float mRangedCoolDownTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float mMeleeCoolDown;


	float mTimeLeftOnCoolDown;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool mIsOnCooldown;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool mIsAttacking;
};
