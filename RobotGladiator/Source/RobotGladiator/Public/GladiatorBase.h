// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../RobotGladiatorCharacter.h"
#include "GladiatorBase.generated.h"

/**
GladiatorBase.h
Purpose: base class for the gladiator boss
Dependencies: RobotGladiator.h/cpp  probably will change this to pawn eventually
Primary Author: Christopher Foster
References: <For the love of god>

 */
UCLASS()
class ROBOTGLADIATOR_API AGladiatorBase : public ARobotGladiatorCharacter
{
	GENERATED_BODY()

	/**  The current target on the gladiator*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AActor* mpTarget;

	AActor* GetClosestPlayer(TArray<AActor*> Array);

public:
	AGladiatorBase();


protected:
	virtual void BeginPlay() override;
	
};
