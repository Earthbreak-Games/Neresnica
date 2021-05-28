// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

UENUM(BlueprintType)
enum EWeaponTypes
{
	SWORD		UMETA(DisplayName = "Sword"),
	AXE			UMETA(DisplayName = "Axe"),
	SHIELD		UMETA(DisplayName = "Shield"),
	SPEAR		UMETA(DisplayName = "Spear"),
	KNIVES		UMETA(DisplayName = "Knives")
};

UCLASS()
class ROBOTGLADIATOR_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
