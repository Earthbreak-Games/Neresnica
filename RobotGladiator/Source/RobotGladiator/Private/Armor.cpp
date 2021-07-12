/**
 * @file Armor.cpp
 * @brief Defines the specific functionality of a piece of armor
 * @dependencies None
 *
 * @author Ethan Heil
 * @credits
 **/

#include "Armor.h"

AArmor::AArmor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Armor Mesh"));
}

void AArmor::SetArmorStats(int defense, ERarities rarity, EArmorTypes type)
{
	// Set stats
	DefenseStat = defense;
	Rarity = rarity;
	ArmorType = type;
}

void AArmor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AArmor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	DOREPLIFETIME(AArmor, DefenseStat);
	DOREPLIFETIME(AArmor, ArmorName);
	DOREPLIFETIME(AArmor, Rarity);
	DOREPLIFETIME(AArmor, ArmorType);
}

void AArmor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

