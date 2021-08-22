// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Core.h"
#include "Navigation/NavLinkProxy.h"
#include "NavLinkCustomComponent.h"
#include "UObject/UObjectGlobals.h"
#include <string>

#include "MyNavLinkProxy.generated.h"




/**
 * 
 */



UCLASS()
// link points are found at base_ptr->GetData()
// smart link points are found at GetSmartLinkComp()->IsEnabled()
class ROBOTGLADIATOR_API AMyNavLinkProxy : public ANavLinkProxy
{
	GENERATED_BODY()

	AMyNavLinkProxy();
	void BeginPlay();
	TArray<FNavigationLink>* base_ptr; //cursed pointer

public:

	UFUNCTION(BlueprintCallable)
	void Set_Jump_Points(FVector left, FVector right);

};



