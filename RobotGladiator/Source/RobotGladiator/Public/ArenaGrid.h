/*
* ArenaGrid.h
* Purpose: Declares the Arena Grid class which is responsible for generating and managing a hexagonal grid
* Dependencies: HexCell.h
* Primary Author: Ethan Heil
*/

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Kismet/KismetMathLibrary.h"
#include "HexCell.h"
#include "ArenaGrid.generated.h"

UCLASS()
class ROBOTGLADIATOR_API AArenaGrid : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AArenaGrid();

	// Spawns initialized grid into the scene
	UFUNCTION(BlueprintCallable)
	void SpawnFloor(FVector origin, int radius, float padding);

	// Clears the currently generated floor
	UFUNCTION(BlueprintCallable)
	void ClearFloor();

	// Initializes the hex grid's data
	void InitHexGrid(int radius);

public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AActor> FloorPieceActor;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<AActor*> FloorPieces;

	TArray<HexCell> Cells;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Radius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Padding;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
