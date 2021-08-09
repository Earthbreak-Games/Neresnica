// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Navigation/NavLinkProxy.h"
#include "DynamicNavLink.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTGLADIATOR_API ADynamicNavLink : public ANavLinkProxy
{
	GENERATED_BODY()
	

public:
	void SetAndEnableSmartLink();
	void ReceiveSmartLinkReached(AActor* Agent, const FVector& Destination);
};
