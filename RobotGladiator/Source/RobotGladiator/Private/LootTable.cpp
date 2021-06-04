/*
* LootTable.h
* Purpose: Defines the functionality of a loot table 
* Dependencies: None
* Primary Author: Ethan Heil
*/

#include "LootTable.h"

/*
* ULootTable
* Default Constructor for the loot table
*/
ULootTable::ULootTable()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	CommonPercentage = 40.0f;
	RarePercentage = 30.0f;
	LegendaryPercentage = 20.0f;
	EpicPercentage = 10.0f;
}

/*
* BeginPlay
* Called at the start of a scene, used for initialization
*/
void ULootTable::BeginPlay()
{
	Super::BeginPlay();
}

/*
* ChooseRarity
* Chooses a rarity for an item. Each rarity has a weighted percentage
*	-Param modifier: decreases the chance of getting a lower tier rarity
*/
ERarities ULootTable::ChooseRarity(float modifier = 0.0f)
{
	// Get random number between 0 and 100
	float randPercent = FMath::RandRange(0, 100);
	randPercent = FMath::Floor(randPercent);

	// Calculate rarities
	if(randPercent <= (CommonPercentage - modifier))
		return COMMON;
	else if(randPercent <= (RarePercentage + CommonPercentage) - modifier)
		return RARE;
	else if(randPercent <= (LegendaryPercentage + RarePercentage + CommonPercentage) - modifier)
		return LEGENDARY;
	else if (randPercent <= (EpicPercentage + LegendaryPercentage + RarePercentage + CommonPercentage) - modifier)
		return EPIC;

	return COMMON;
}

/*
* ChooseItemType
* Chooses an item type based on the type of enemy.
*	-Param enemyType: the type of enemy
*	-Param modifier: decreases the chance of getting a lower tier item
*/
ELootTypes ULootTable::ChooseItemType(EEnemyTypes enemyType, float modifier = 0.0f)
{
	// Grunt loot drops
	if (enemyType == GRUNT)
	{
		// Get random number between 0 and 100
		float randPercent = FMath::RandRange(0, 100);
		randPercent = FMath::Floor(randPercent);

		// Choose item
		if(randPercent <= 70.0f - modifier)
			return HEALTH;
		else if(randPercent <= 100.0f - modifier)
			return ARMOR;
	}
	// Boss Loot Drops
	else if (enemyType == BOSS)
	{
		return WEAPON;
	}

	return WEAPON;
}

/*
* TickComponent
* Called every frame
*/
void ULootTable::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

