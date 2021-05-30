// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LootEnums.h"
#include "Weapon.generated.h"



UCLASS()
class ROBOTGLADIATOR_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

	UFUNCTION(BlueprintCallable)
	void SetWeaponStats(int baseDamage, ERarities rarity, EWeaponTypes type);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* Mesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = "true"))
	FName WeaponName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<ERarities> Rarity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<EWeaponTypes> WeaponType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int BaseDamage;
};
