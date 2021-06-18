/**
 * @file Armor.h
 * @brief Declares the specific functionality of a piece of armor
 * @dependencies LootEnums.h
 *
 * @author Ethan Heil
 * @credits
 **/

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LootEnums.h"
#include "Armor.generated.h"


UCLASS()
class ROBOTGLADIATOR_API AArmor : public AActor
{
	GENERATED_BODY()
	
public:	
	/** @brief Default Constructor for an armor piece
	 */
	AArmor();

	UFUNCTION(BlueprintCallable)
	/** @brief Sets the stats for this armor piece
	 *  @param {int} defense - Base defense of the armor piece
	 *  @param {ERarities} rarity - Rarity of the armor
	 *  @param {EArmorTypes} type - The armor type
	 */
	void SetArmorStats(int defense, ERarities rarity, EArmorTypes type);

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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn="true"))
	FName ArmorName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<ERarities> Rarity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<EArmorTypes> ArmorType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int DefenseStat;
};
