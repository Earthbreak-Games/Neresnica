/*
* Weapon.cpp
* Purpose: Defines the specific functionality of a weapon
* Dependencies: None
* Primary Author: Ethan Heil
*/

#include "Weapon.h"
#include "LootHUD.h"

/*
* AWeapon
* Default Constructor for a weapon
*/
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon Mesh"));
}

/*
* SetWeaponStats
* Sets the stats for this weapon
*	-Param baseDamage: base damage of the weapon
*	-Param rarity: rarity of the weapon
*	-Param type: the weapon type
*/
void AWeapon::SetWeaponStats(int baseDamage, ERarities rarity, EWeaponTypes type)
{
	// Set stats
	BaseDamage = baseDamage;
	Rarity = rarity;
	WeaponType = type;
}

/*
* BeginPlay
* Called at the start of a scene, used for initialization
*/
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
}

/*
* Tick
* Called every frame
*/
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

