// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NavLinkComponent.h"
#include "MyNavLinkComponent.generated.h"

/**
 * 
 */
UCLASS(meta = (BlueprintSpawnableComponent))
class ROBOTGLADIATOR_API UMyNavLinkComponent : public UNavLinkComponent
{
	GENERATED_BODY()
	UFUNCTION(BlueprintCallable)

	void SetupLink(FVector Left, FVector Right, ENavLinkDirection::Type Direction);
	
};
