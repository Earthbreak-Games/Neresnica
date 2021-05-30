// Fill out your copyright notice in the Description page of Project Settings.

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
	KNIVES		UMETA(DisplayName = "Knives")
};

UENUM(BlueprintType)
enum EArmorTypes
{
	HEAD		UMETA(DisplayName = "Head"),
	CHEST		UMETA(DisplayName = "Chest"),
	ARMS		UMETA(DisplayName = "Arms"),
	LEGS		UMETA(DisplayName = "Legs"),
};
