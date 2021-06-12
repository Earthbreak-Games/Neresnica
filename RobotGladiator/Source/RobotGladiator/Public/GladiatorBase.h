// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../RobotGladiatorCharacter.h"
#include "GladiatorBase.generated.h"

/**
GladiatorBase.h
Purpose: base class for the gladiator boss
Dependencies: APawn.h/cpp  
Primary Author: Christopher Foster
References: <For the love of god>

 */
UCLASS()
class ROBOTGLADIATOR_API AGladiatorBase : public ACharacter
{
	GENERATED_BODY()

	/**  The current target on the gladiator*/
	//UFUNCTION
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
	float mHealth;

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
