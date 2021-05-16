/*
* ArenaGrid.cpp
* Purpose: Defines functionality of the Arena Grid class which is responsible for generating and managing a hexagonal grid
* Dependencies: ArenaGrid.h, HexCell.h
* Primary Author: Ethan Heil
*/

#include "ArenaGrid.h"

// Sets default values
AArenaGrid::AArenaGrid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Radius = 1.0f;
	Padding = 1.0f;
}

/*
* Spawn Floor
* Spawns a hexagonal grid in the world at a given point
*	- Param origin: origin point of the grid (aka the center of the grid)
*	- Param radius: the radius of the grid
*	- Param padding the amount of padding between each cell in the grid
*/
void AArenaGrid::SpawnFloor(FVector origin, int radius, float padding)
{
	// Initialize hex grid data
	InitHexGrid(radius);

	// Loop through all initialized cells
	for (int i = 0; i < Cells.Num(); i++)
	{
		// Init spawn parameters
		FActorSpawnParameters spawnParams;
		spawnParams.Owner = this;
		spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		// Set spawn location
		FVector spawnLoc = origin;

		// Check if actor to spawn is valid
		if (FloorPieceActor)
		{
			// Get current cell
			HexCell currentCell = CubeToAxial(Cells[i]);
			
			// Calculate tile offset. See https://www.redblobgames.com/grids/hexagons/ (Hex to pixel section) for more info on coordinate conversion
			float xPos = padding * (FMath::Sqrt(3) * currentCell.GetQ() + FMath::Sqrt(3) / 2 * currentCell.GetR());
			float yPos = padding * (3.0f/ 2.0f * currentCell.GetR());
			FVector tileOffset = FVector(xPos, yPos, GetTransform().GetLocation().Z) * padding;

			// Spawn new tile
			FRotator rot = this->GetActorRotation();
			
			AActor* floorPiece = GetWorld()->SpawnActor<AActor>(FloorPieceActor, spawnLoc + tileOffset, rot, spawnParams);

			rot = FRotator(0.0f, 30.0f, 0.0f);
			floorPiece->AddActorLocalRotation(rot);
			FloorPieces.Add(floorPiece);
		}
	}
}

/*
* Clear Floor
* Clears the currently generated grid from the scene and resets all grid data
*/
void AArenaGrid::ClearFloor()
{
	// Loop through all floor pieces and destroy them
	for (int i = 0; i < FloorPieces.Num(); i++)
	{
		// Check if floor piece is valid
		if (FloorPieces[i])
		{
			// Destroy current floor piece
			FloorPieces[i]->Destroy();
		}
	}

	// Clear FloorPieces and Cells arrays
	FloorPieces.Empty();
	Cells.Empty();
}

/*
* InitHexGrid
* Initializes the hex grid's data
*	- Param radius: the radius of the hex grid
* See https://www.redblobgames.com/grids/hexagons/implementation.html for more info 
* on grid generation algorithms
*/
void AArenaGrid::InitHexGrid(int radius)
{
	// Loop through entire column range
	for (int col = -radius; col <= radius; col++)
	{
		// Calculate max and min rows on current column
		int rowMax = FMath::Max(-radius, -col - radius);
		int rowMin = FMath::Min(radius, -col + radius);

		// Loop through all rows on this column
		for (int row = rowMax; row <= rowMin; row++)
		{	
			// Add new cell with row and column data
			Cells.Add(HexCell(col, row, -col - row));
		}
	}
}

// Called when the game starts or when spawned
void AArenaGrid::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AArenaGrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

