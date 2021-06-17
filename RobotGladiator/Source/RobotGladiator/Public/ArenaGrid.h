/*
* ArenaGrid.h
* Purpose: Declares the Arena Grid class which is responsible for generating and managing a hexagonal grid
* Dependencies: HexCell.h
* Primary Author: Ethan Heil
*	State Saving/Editing Additions: Henry Chronowski
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString mName;

	FSaveState(TArray<float> in) : mHeights(in)
	{
		mName = "";
	}

	// Sets default values for this actor's properties
	FSaveState()
	{
		mHeights.AddZeroed();
		mName = "";
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
	int SaveState(int index = -1, bool freshState = true);

	// Generate new tile heights from in the editor
	UFUNCTION(BlueprintCallable)
	void GenerateHeights(float scale = 1.0f);

	// Remove the stored height at the given index
	UFUNCTION(BlueprintCallable)
	void EraseHeightState(int index);

	// Loads the saved state at the given index
	UFUNCTION(BlueprintCallable)
	void LoadSaveState(int index, FVector origin, int radius, float padding);

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
	void CalculateTilePositions(float scale = 1.0f);

	void CalculateRing(HexCell center, int radius);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


private:
	// A random stream to seed the perlin noise sample
	FRandomStream mRand;
};
