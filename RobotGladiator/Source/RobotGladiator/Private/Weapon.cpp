/**
 * @file Weapon.cpp
 * @brief Defines the specific functionality of a weapon
 * @dependencies None
 *
 * @author Ethan Heil
 **/

#include "Weapon.h"
#include "LootHUD.h"

AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon Mesh"));
}

void AWeapon::SetWeaponStats(int baseDamage, ERarities rarity, EWeaponTypes type)
{
	// Set stats
	BaseDamage = baseDamage;
	Rarity = rarity;
	WeaponType = type;
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();
}

void AWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AWeapon, BaseDamage);
	DOREPLIFETIME(AWeapon, Rarity);
	DOREPLIFETIME(AWeapon, WeaponType);
	DOREPLIFETIME(AWeapon, WeaponName);
}

void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

