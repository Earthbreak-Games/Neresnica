// Fill out your copyright notice in the Description page of Project Settings.

/**
 * @file BaseUnit.h
 * @brief the base c++ class to contain health and functions related to taking and dealing damage
 * @dependencies Character.h
 *
 * @author Chris Foster
 * @credits
 **/

#include "BaseUnit.h"

// Sets default values
ABaseUnit::ABaseUnit()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABaseUnit::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABaseUnit::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

/**   @brief Take damage and decrement mHealth
 *    @param {float} damage - damage to be taken
 *    @return {void} null
 */
void ABaseUnit::TakeDamage(float damage)
{
	mHealth -= damage;

	if (mHealth <= 0)
	{
		Destroy();
	}
}


/**   @brief <heal>
 *    @param {<float>} hp - health
 *    @return {<void>} null
 */
void ABaseUnit::Heal(float hp)
{
	mHealth += hp;

	if (mHealth > mMaxHealth)
	{
		mHealth = mMaxHealth;
	}
}

/**   @brief <heal>
 *	  @param {ABaseUnit*} oposingUnit - <unit to take damage>
 *    @param {<float>} damage - damage>
 *    @return {<void>} - <null>
 */
void ABaseUnit::DealDamage(ABaseUnit* oposingUnit, float damage)
{
	oposingUnit->TakeDamage(damage);
}
