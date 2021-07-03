/**
 * @file LootTable.h
 * @brief Declares the specific functionality of a loot table
 * @dependencies Weapon.h, Armor.h, LootEnums.h
 *
 * @author Ethan Heil
 **/

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Weapon.h"
#include "Armor.h"
#include "LootEnums.h"
#include "LootTable.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROBOTGLADIATOR_API ULootTable : public UActorComponent
{
	GENERATED_BODY()

public:	
	/** @brief Default Constructor for the loot table
	 */
	ULootTable();

	UFUNCTION(BlueprintCallable)
	/** @brief Chooses a rarity for an item. Each rarity has a weighted percentage
	 *  @param {float} modifier - Decreases the chance of getting a lower tier rarity
	 */
	ERarities ChooseRarity(float modifier);

	UFUNCTION(BlueprintCallable)
	/** @brief Chooses an item type based on the type of enemy.
	 *  @param {EEnemyTypes} enemyType - The type of enemy
	 *  @param {float} modifier - Decreases the chance of getting a lower tier item
	 */
	ELootTypes ChooseItemType(EEnemyTypes enemyType, float modifier);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FString, float> Table;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CommonPercentage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RarePercentage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LegendaryPercentage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float EpicPercentage;

protected:
	/** @brief Called at the start of a scene, used for initialization
	 */
	virtual void BeginPlay() override;

public:	
	/** @brief Called every frame
	 */
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
