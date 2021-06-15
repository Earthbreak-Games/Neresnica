/*
* Armor.h
* Purpose: Declares the specific functionality of a piece of armor
* Dependencies: LootEnums.h
* Primary Author: Ethan Heil
*/

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LootEnums.h"
#include "Armor.generated.h"


UCLASS()
class ROBOTGLADIATOR_API AArmor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AArmor();

	// Sets the stats for this armor piece
	UFUNCTION(BlueprintCallable)
	void SetArmorStats(int defense, ERarities rarity, EArmorTypes type);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* Mesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn="true"))
	FName ArmorName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<ERarities> Rarity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<EArmorTypes> ArmorType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int DefenseStat;
};
