// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Net/UnrealNetwork.h"
#include "LootEnums.h"
#include "Upgrade.generated.h"

UCLASS()
class ROBOTGLADIATOR_API AUpgrade : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AUpgrade();

	UFUNCTION(BlueprintCallable)
	/** @brief Sets the stats for this weapon
	 *  @param {int} baseDamage - Base damage of the weapon
	 *  @param {ERarities} rarity - Rarity of the weapon
	 *  @param {EWeaponTypes} type - The weapon type
	 */
	void SetUpgradeStats(ERarities rarity, EUpgradeStatType stat, EWeaponTypes affectedWeapon, float statIncrease, float stackIncrease);

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* Mesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = "true"), Replicated)
	FName UpgradeName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	TEnumAsByte<ERarities> Rarity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	TEnumAsByte<EUpgradeStatType> UpgradeStat;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	TEnumAsByte<EWeaponTypes> AffectedWeapon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	float StatIncrease;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	float StackIncrease;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
