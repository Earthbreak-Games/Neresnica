/*
* Armor.cpp
* Purpose: Defines the specific functionality of a piece of armor
* Dependencies: None
* Primary Author: Ethan Heil
*/

#include "Armor.h"

/*
* AArmor
* Default Constructor for an armor piece
*/
AArmor::AArmor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Armor Mesh"));
}

/*
* SetArmorStats
* Sets the stats for this armor piece
*	-Param defense: base defense of the armor piece
*	-Param rarity: rarity of the armor
*	-Param type: the armor type
*/
void AArmor::SetArmorStats(int defense, ERarities rarity, EArmorTypes type)
{
	// Set stats
	DefenseStat = defense;
	Rarity = rarity;
	ArmorType = type;
}

/*
* BeginPlay
* Called at the start of a scene, used for initialization
*/
void AArmor::BeginPlay()
{
	Super::BeginPlay();
	
}

/*
* Tick
* Called every frame
*/
void AArmor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

