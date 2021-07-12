/**
 * @file Weapon.h
 * @brief Declares the specific functionality of a weapon
 * @dependencies LootEnums.h
 *
 * @author Ethan Heil
 **/

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Net/UnrealNetwork.h"
#include "LootEnums.h"
#include "Weapon.generated.h"

UCLASS()
class ROBOTGLADIATOR_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	/** @brief Default Constructor for a weapon
	 */
	AWeapon();

	UFUNCTION(BlueprintCallable)
	/** @brief Sets the stats for this weapon
	 *  @param {int} baseDamage - Base damage of the weapon
	 *  @param {ERarities} rarity - Rarity of the weapon
	 *  @param {EWeaponTypes} type - The weapon type
	 */
	void SetWeaponStats(int baseDamage, ERarities rarity, EWeaponTypes type);

protected:
	/** @brief Called at the start of a scene, used for initialization
	 */
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

public:	
	/** @brief Called every frame
	 */
	virtual void Tick(float DeltaTime) override;

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* Mesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = "true"), Replicated)
	FName WeaponName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	TEnumAsByte<ERarities> Rarity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	TEnumAsByte<EWeaponTypes> WeaponType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	int BaseDamage;

protected:
	
};
