/*
* LootTable.h
* Purpose: Declares the specific functionality of a loot table
* Dependencies: Weapon.h, Armor.h, LootEnums.h
* Primary Author: Ethan Heil
*/

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
	// Sets default values for this component's properties
	ULootTable();

	// Choose rarity for an item
	UFUNCTION(BlueprintCallable)
	ERarities ChooseRarity(float modifier);
	// Choose item type based on enemy
	UFUNCTION(BlueprintCallable)
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
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
