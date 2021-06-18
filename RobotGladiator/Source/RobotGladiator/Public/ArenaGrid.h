/**
 * @file ArenaGrid.h
 * @brief Declares the Arena Grid class which is responsible for generating and managing a hexagonal grid
 * @dependencies HexCell.h
 *
 * @author Ethan Heil
 * @author Henry Chronowski - State Saving/Editing
 * @credits
 *	https://www.redblobgames.com/grids/hexagons/
 **/

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
	AArenaGrid();

	/** @brief Spawns a hexagonal grid in the world at a given point
	*  @param {FVector} origin - Origin point of the grid (aka the center of the grid)
	*  @param {int} radius - The radius of the grid
	*  @param {float} padding - The amount of padding between each cell in the grid
	*/
	UFUNCTION(BlueprintCallable)
	void SpawnFloor(FVector origin, int radius, float padding);

	/** @brief Clears the currently generated grid from the scene and resets all grid data
	*/
	UFUNCTION(BlueprintCallable)
	void ClearFloor();

	/** @brief Initializes the hex grid's data
	*  @param {int} radius - The radius of the hex grid
	*  @param {int} radius - The radius of the grid
	*  @references See https://www.redblobgames.com/grids/hexagons/ (Rings Section) for more info
	*		on grid generation algorithms
	*/
	void InitHexGrid(int radius);

	/** @brief Activates the starting sequence for the current round
	*/
	UFUNCTION(BlueprintCallable)
	void StartRound();

	/** @brief Activates the ending sequence for the current round
	*/
	UFUNCTION(BlueprintCallable)
	void EndRound();

	/** @brief Save the current hex grid state to the saved state array at the specified index.
	*  @param {int} [index=-1] - The index to save at. Default or invalid appends
	*  @param {bool} freshState - This governs whether current editor data is saved or ignored
	*  @return {int} - The index at which the new save state was stored
	*/
	UFUNCTION(BlueprintCallable)
	int SaveState(int index = -1, bool freshState = true);

	/** @brief Calls the calculate tile positions protected function in order to randomize heights
	*  @param {float} scale - A float scale factor for the Perlin noise sample
	*/
	UFUNCTION(BlueprintCallable)
	void GenerateHeights(float scale = 1.0f);

	/** @brief Erases the save state stored at the given index
	*  @param {int} index - The index of the saved state to erase
	*/
	UFUNCTION(BlueprintCallable)
	void EraseHeightState(int index);

	/** @brief Loads the save state stored at the given index. If there is an invalid index loads the
	 *		default arena map.
	 *	This is effectively SpawnFloor but with stored level data rather than defaults
	 *  @param {int} index - The index of the saved state to load
	 *  @param {FVector} origin - Origin point of the grid (aka the center of the grid)
	 *  @param {int} radius - The radius of the grid
	 *  @param {float} padding - The amount of padding between each cell in the grid
	 */
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
	virtual void BeginPlay() override;

	/** @brief Calculates the position of each tile using simplex noise
	 */
	void CalculateTilePositions(float scale = 1.0f);

	/** @brief Calculates a ring around a tile with a given radius
	 *  @param {HexCell} center - The center tile of the ring
	 *  @param {int} radius - The radius of the ring (in tiles)
	 *  @references See https://www.redblobgames.com/grids/hexagons/ (Rings Section) for more info
	 */
	void CalculateRing(HexCell center, int radius);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


private:
	// A random stream to seed the perlin noise sample
	FRandomStream mRand;
};
