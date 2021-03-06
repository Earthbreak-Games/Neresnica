// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseUnit.h"
#include "GruntBase.generated.h"

/**
GruntBase.h
Purpose: base class for the grunts
Dependencies: APawn.h/cpp  
Primary Author: Aidan Murphy
References: Foster code for getting nearest unit, and movement in order to make sure that jumping will function properly
*/

UCLASS()
class ROBOTGLADIATOR_API AGruntBase : public ABaseUnit
{
	GENERATED_BODY()
	

	/** @brief Get the closes player to the gladiator
	*	@param {TArray<AActor*>} Array - array of actors to choose from -
	*   @return {AActor*} - the closest player to the gladiator
	*/
	// AActor* GetClosestPlayer(TArray<AActor*> Array);

protected:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:

	AGruntBase();

	UFUNCTION(BlueprintCallable, Category="grunt")
		AActor* GetClosestPlayer(TArray<AActor*> Array);

};
