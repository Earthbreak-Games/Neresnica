// Fill out your copyright notice in the Description page of Project Settings.

/**
 * @file BaseUnit.h
 * @brief the base c++ class to contain health and functions related to taking and dealing damage
 * @dependencies Character.h
 *
 * @author Chris Foster
 * @credits
 **/

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseUnit.generated.h"

UCLASS()
class ROBOTGLADIATOR_API ABaseUnit : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABaseUnit();


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
		float mHealth;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
		float mMaxHealth;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* 
		PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	/**   @brief Take damage and decrement mHealth
	*    @param {float} damage - damage to be taken
	*    @return {void} null
	*/
	void TakeDamage(float damage);

	UFUNCTION(BlueprintCallable)
	/**   @brief <heal>
	*    @param {<float>} hp - health
	*    @return {<void>} null
	*/
	void Heal(float hp);

	UFUNCTION(BlueprintCallable)
	/**   @brief <heal>
	*	  @param {ABaseUnit*} oposingUnit - <unit to take damage>
	*    @param {<float>} damage - damage>
	*    @return {<void>} - <null>
	*/
	void DealDamage(ABaseUnit* oposingUnit, float damage);
	
};
