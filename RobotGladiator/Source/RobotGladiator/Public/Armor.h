// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Armor.generated.h"

UENUM(BlueprintType)
enum EArmorTypes
{
	HEAD		UMETA(DisplayName = "Head"),
	CHEST		UMETA(DisplayName = "Chest"),
	ARMS		UMETA(DisplayName = "Arms"),
	LEGS		UMETA(DisplayName = "Legs"),
};

UCLASS()
class ROBOTGLADIATOR_API AArmor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AArmor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
