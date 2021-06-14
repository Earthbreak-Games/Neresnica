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
#include "Math/UnrealMathUtility.h"
#include "ArenaGrid.generated.h"

USTRUCT(BlueprintType)
struct FSaveState
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<float> mHeights;

	FSaveState(TArray<float> in) : mHeights(in)
	{
	}

	FSaveState()
	{
		mHeights.AddZeroed();
	}
};

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

	// Activate starting sequence for the current round
	UFUNCTION(BlueprintCallable)
	void StartRound();

	// Activate ending sequence for the current round
	UFUNCTION(BlueprintCallable)
	void EndRound();

	// Save the current hex grid state to the saved state array at the specified index
	UFUNCTION(BlueprintCallable)
	void SaveState(int index = -1);

	// Generate new tile heights from in the editor
	UFUNCTION(BlueprintCallable)
	void GenerateHeights();

	// Remove the stored height at the given index
	UFUNCTION(BlueprintCallable)
	void EraseHeightState(int index);

public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AActor> FloorPieceActor;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<AActor*> FloorPieces;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<float> FloorHeights;
	TArray<HexCell> Cells;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<FSaveState> SavedStates;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Radius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Padding;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MinHeight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHeight;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Calculate new tile position using Simplex Noise
	void CalculateTilePositions();

	void CalculateRing(HexCell center, int radius);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
