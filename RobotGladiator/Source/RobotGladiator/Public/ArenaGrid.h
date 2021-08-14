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

#define DEBUGMESSAGE(x, ...) if(GEngine){GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, FString::Printf(TEXT(x), __VA_ARGS__));}

USTRUCT(BlueprintType)
/** @brief A struct encompassing the data saved for each hex cell
 */
struct FSaveState
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<float> mHeights;								// It might be usefull to store xyz for spawning modifiers
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString mName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<int> mModifiers;

	static enum ModifierIDs
	{
		NONE = 0,
		HEAL_TOPPER,
		TOXIC_TOPPER,
		JUMP_TOPPER,
		GRUNT,

		NUM_MODIFIERS,

		GLADIATOR			// This is at the end so that it can be stored in the modifier system but not factored into modifier chance
	};

	FSaveState(TArray<float> inHeights, TArray<int> inMods) : mHeights(inHeights), mModifiers(inMods)
	{
		mName = "";
	}

	// Sets default values for this actor's properties
	FSaveState()
	{
		mHeights.AddZeroed();
		mName = "";
		mModifiers.AddZeroed();
	}

	// Sets default values for an arena size
	FSaveState(int size)
	{
		mHeights.AddZeroed(size);
		mName = "";
		mModifiers.AddZeroed(size);
	}
};

UCLASS()
class ROBOTGLADIATOR_API AArenaGrid : public AActor
{
	GENERATED_BODY()

public:	
	AArenaGrid();

	UFUNCTION(BlueprintCallable)
	/** @brief Spawns a hexagonal grid in the world at a given point
	*  @param {FVector} origin - Origin point of the grid (aka the center of the grid)
	*  @param {int} radius - The radius of the grid
	*  @param {float} padding - The amount of padding between each cell in the grid
	*/
	void SpawnFloor(FVector origin, int radius, float padding);

	UFUNCTION(BlueprintCallable)
	/** @brief Clears the currently generated grid from the scene and resets all grid data
	*/
	void ClearFloor();

	/** @brief Initializes the hex grid's data
	*  @param {int} radius - The radius of the hex grid
	*  @param {int} radius - The radius of the grid
	*  @references See https://www.redblobgames.com/grids/hexagons/ (Rings Section) for more info
	*		on grid generation algorithms
	*/
	void InitHexGrid(int radius);

	UFUNCTION(BlueprintCallable)
	/** @brief Activates the starting sequence for the current round
	*/
	void StartRound();

	UFUNCTION(BlueprintCallable)
	/** @brief Activates the ending sequence for the current round
	*/
	void EndRound();

	UFUNCTION(BlueprintCallable)
	void SetupLobbyOrientation(int numTiles);

	UFUNCTION(BlueprintCallable)
	/** @brief Save the current hex grid state to the saved state array at the specified index.
	 *  @param {int} [index=-1] - The index to save at. Default or invalid appends
	 *  @param {bool} freshState - This governs whether current editor data is saved or ignored
	 *  @return {int} - The index at which the new save state was stored
	 */
	int SaveState(int index = -1, bool freshState = true);

	UFUNCTION(BlueprintCallable)
	/** @brief Calls the calculate tile positions protected function in order to randomize heights
	*  @param {float} scale - A float scale factor for the Perlin noise sample, scaled by 0.001 in the math
	*/
	void GenerateArena(float chance, float scale = 1.0f);

	UFUNCTION(BlueprintCallable)
	/** @brief Erases the save state stored at the given index
	*  @param {int} index - The index of the saved state to erase
	*/
	void EraseHeightState(int index);

	UFUNCTION(BlueprintCallable)
	/** @brief Loads the save state stored at the given index in the arena editor.
	 *		If there is an invalid index loads the default arena map.
	 *	This is effectively SpawnFloor but with stored level data rather than defaults
	 *  @param {int} index - The index of the saved state to load
	 *  @param {FVector} origin - Origin point of the grid (aka the center of the grid)
	 *  @param {int} radius - The radius of the grid
	 *  @param {float} padding - The amount of padding between each cell in the grid
	 */
	FSaveState EditorLoadSaveState(int index, FVector origin, int radius, float padding);

	UFUNCTION(BlueprintCallable)
	/** @brief Loads the next level during gameplay. Loads from the saved state if any remain,
	 *		if not generates a new level from scratch.
	 *  @param {int} index - The number of the current arena state
	 *  @param {float} scale - A float scale factor for the Perlin noise sample, scaled by 0.001 in the math
	 */
	FSaveState LoadSaveStateData(UPARAM(ref) int&index, float scale);

	UFUNCTION(BlueprintCallable)
	/** @brief Spawns modifiers during gameplay
	 *  @param {FSaveState} cur - The saved state to load modifier data from
	 */
	void LoadModifiers(UPARAM(ref) FSaveState cur);

	UFUNCTION(BlueprintCallable)
	/** @brief Empties the modifier actor arrays, cleaning up the board
	 */
	void ClearTheBoard();

public:
	UPROPERTY(EditAnywhere,Category=Actors)
	TSubclassOf<class AActor> FloorPieceActor;
	UPROPERTY(EditAnywhere,Category=Actors)
	TSubclassOf<class AActor> Gladiator;
	UPROPERTY(EditAnywhere,Category=Actors)
	TSubclassOf<class AActor> Grunt;
	UPROPERTY(EditAnywhere,Category=Actors)
	TSubclassOf<class AActor> healTopper;
	UPROPERTY(EditAnywhere,Category=Actors)
	TSubclassOf<class AActor> jumpTopper;
	UPROPERTY(EditAnywhere,Category=Actors)
	TSubclassOf<class AActor> toxicTopper;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category=CurrentArenaComponents)
	TArray<AActor*> FloorPieces;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category=CurrentArenaComponents)
	TArray<AActor*> Enemies;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category=CurrentArenaComponents)
	TArray<AActor*> Toppers;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category=CurrentArenaComponents)
	TArray<float> FloorHeights;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category=CurrentArenaComponents)
	TArray<int> FloorModifiers;
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

	/** @brief Calculates the modifiers on each tile
	 *	@param {float} chance - The probability that a given tile will not have a modifier, as a percentage out of 100
	 */
	void CalculateTileModifiers(float chance);

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
