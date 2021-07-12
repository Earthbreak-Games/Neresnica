/**
 * @file LootEnums.h
 * @brief Declares all enums that deal with loot and rarities
 * @dependencies None
 *
 * @author Ethan Heil
 **/

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum ERarities
{
	COMMON		UMETA(DisplayName = "Common"),
	RARE		UMETA(DisplayName = "Rare"),
	LEGENDARY	UMETA(DisplayName = "Legendary"),
	EPIC		UMETA(DisplayName = "Epic")
};

UENUM(BlueprintType)
enum ELootTypes
{
	HEALTH		UMETA(DisplayName = "Health"),
	WEAPON		UMETA(DisplayName = "Weapon"),
	ARMOR		UMETA(DisplayName = "Armor")
};

UENUM(BlueprintType)
enum EEnemyTypes
{
	GRUNT		UMETA(DisplayName = "Grunt"),
	BOSS		UMETA(DisplayName = "Boss")
};

UENUM(BlueprintType)
enum EWeaponTypes
{
	SWORD		UMETA(DisplayName = "Sword"),
	AXE			UMETA(DisplayName = "Axe"),
	SHIELD		UMETA(DisplayName = "Shield"),
	SPEAR		UMETA(DisplayName = "Spear"),
	KNIVES		UMETA(DisplayName = "Knives"),
	FIST		UMETA(DisplayName = "Fist")
};

UENUM(BlueprintType)
enum EArmorTypes
{
	HEAD		UMETA(DisplayName = "Head"),
	CHEST		UMETA(DisplayName = "Chest"),
	ARMS		UMETA(DisplayName = "Arms"),
	LEGS		UMETA(DisplayName = "Legs"),
};
