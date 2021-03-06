// Fill out your copyright notice in the Description page of Project Settings.


#include "Upgrade.h"

// Sets default values
AUpgrade::AUpgrade()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Upgrade Mesh"));
}

void AUpgrade::SetUpgradeStats(ERarities rarity, EUpgradeStatType stat, EWeaponTypes affectedWeapon, float statIncrease, float stackIncrease)
{
	Rarity = rarity;
	UpgradeStat = stat;
	AffectedWeapon = affectedWeapon;
	StatIncrease = statIncrease;
	StackIncrease = stackIncrease;
}

// Called when the game starts or when spawned
void AUpgrade::BeginPlay()
{
	Super::BeginPlay();
	
}

void AUpgrade::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AUpgrade, UpgradeName);
	DOREPLIFETIME(AUpgrade, Rarity);
	DOREPLIFETIME(AUpgrade, AffectedWeapon);
	DOREPLIFETIME(AUpgrade, UpgradeStat);
	DOREPLIFETIME(AUpgrade, StatIncrease);
	DOREPLIFETIME(AUpgrade, StackIncrease);
}

// Called every frame
void AUpgrade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

