// Fill out your copyright notice in the Description page of Project Settings.


#include "LootTable.h"

// Sets default values for this component's properties
ULootTable::ULootTable()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


FString ULootTable::ChooseLoot()
{
	return TEXT("");
}

// Called when the game starts
void ULootTable::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


ERarities ULootTable::ChooseRarity(float modifier = 0.0f)
{
	float randPercent = FMath::RandRange(0, 100);
	randPercent = FMath::Floor(randPercent) / 100.0f; // Set float precision to 2 decimal places

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

ELootTypes ULootTable::ChooseItem(EEnemyTypes enemyType, float modifier = 0.0f)
{
	// Need to check if boss is dropping item, if so guarantee weapon drop

	// Grunt loot drops
	if (enemyType == GRUNT)
	{
		float randPercent = FMath::RandRange(0, 100);
		randPercent = FMath::Floor(randPercent) / 100.0f; // Set float precision to 2 decimal places

		return WEAPON;
	}
	// Boss Loot Drops
	else if (enemyType == BOSS)
	{
		float randPercent = FMath::RandRange(0, 100);
		randPercent = FMath::Floor(randPercent) / 100.0f; // Set float precision to 2 decimal places
	}

	return WEAPON;
}

// Called every frame
void ULootTable::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

