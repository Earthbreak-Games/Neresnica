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

public:	
	/** @brief Called every frame
	 */
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

protected:
	
};
