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

void ABaseUnit::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABaseUnit, mHealth);
	DOREPLIFETIME(ABaseUnit, mMaxHealth);
}

// Called every frame
void ABaseUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

/**   @brief Get the closes player to the gladiator
 *	  @param {TArray<AActor*>} Array - array of actors to choose from -
 *    @return {AActor*} - the closest player to the gladiator
 */
AActor* ABaseUnit::GetClosestPlayer(TArray<AActor*> Array)
{
	AActor* closestPlayer = nullptr;
	float distance = INT_MAX;


	//find the closes actor
	for (AActor* actor : Array)
	{
		if (actor != this)
		{
			if (closestPlayer == nullptr)
			{
				closestPlayer = actor;
				FVector dir = closestPlayer->GetActorLocation() - this->GetActorLocation();
				distance = dir.Size();
			}
			else
			{
				FVector dir = actor->GetActorLocation() - this->GetActorLocation();

				if (dir.SizeSquared() < distance * distance)
				{
					closestPlayer = actor;
					distance = dir.Size();
				}
			}
		}
	}

	return closestPlayer;
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
void ABaseUnit::TakeDamage_Unit(float damage)
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

/**   @brief <Heal an oposing unit>
 *	  @param {ABaseUnit*} oposingUnit - <unit to heal>
 *    @param {<float>} healAmount - amount to heal>
 *    @return {<void>} - <null>
 */
void ABaseUnit::HealUnit(ABaseUnit* oposingUnit, float healAmount)
{
	if(oposingUnit)
		oposingUnit->Heal(healAmount);
}

/**   @brief <Deal damage to an oposing unit>
 *	  @param {ABaseUnit*} oposingUnit - <unit to take damage>
 *    @param {<float>} damage - damage>
 *    @return {<void>} - <null>
 */
void ABaseUnit::DealDamage(ABaseUnit* oposingUnit, float damage)
{
	if(oposingUnit)
		oposingUnit->TakeDamage_Unit(damage);
}

