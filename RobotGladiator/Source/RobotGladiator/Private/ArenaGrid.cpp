/*
* ArenaGrid.cpp
* Purpose: Defines functionality of the Arena Grid class which is responsible for generating and managing a hexagonal grid
* Dependencies: ArenaGrid.h, HexCell.h
* Primary Author: Ethan Heil
*	State Saving/Editing Additions: Henry Chronowski
*/

#include "ArenaGrid.h"

// Sets default values
AArenaGrid::AArenaGrid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Initialize members
	Radius = 1.0f;
	Padding = 1.0f;
	MinHeight = 0.0f;
	MaxHeight = 1500.0f;
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

	HexCell center = HexCell(0.0f, 0.0f, 0.0f);

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
			currentCell.SetID(i);
			
			// Calculate tile offset. See https://www.redblobgames.com/grids/hexagons/ (Hex to pixel section) for more info on coordinate conversion
			float xPos = padding * (FMath::Sqrt(3) * currentCell.GetQ() + FMath::Sqrt(3) / 2 * currentCell.GetR());
			float yPos = padding * (3.0f/ 2.0f * currentCell.GetR());

			FVector tileOffset;
			if (currentCell.GetQ() == 0 && currentCell.GetR() == 0)
			{
				tileOffset = FVector(xPos, yPos, MaxHeight);
				center = currentCell;
			}
			else
				tileOffset = FVector(xPos, yPos, GetTransform().GetLocation().Z) * padding;

			// Spawn new tile
			FRotator rot = this->GetActorRotation();
			AActor* floorPiece = GetWorld()->SpawnActor<AActor>(FloorPieceActor, spawnLoc + tileOffset, rot, spawnParams);

			// Child new floor piece to the grid object
			FAttachmentTransformRules attachRules = FAttachmentTransformRules::KeepWorldTransform;
			floorPiece->AttachToActor(this, attachRules);

			// Set the correct rotation of the floor piece
			rot = FRotator(0.0f, 30.0f, 0.0f);
			floorPiece->AddActorLocalRotation(rot);

			// Add the floor piece to floor piece array
			FloorPieces.Add(floorPiece);
		}
	}

	CalculateRing(center, radius);
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
* See https://www.redblobgames.com/grids/hexagons/ (Rings Section) for more info 
* on grid generation algorithms
*/
void AArenaGrid::InitHexGrid(int radius)
{
	// Create center tile
	HexCell center = HexCell(0.0f, 0.0f, 0.0f);
	Cells.Add(center);
	
	// Create tiles in a ring around the center
	for(int i = 0; i <= radius; i++)
		CalculateRing(center, i);

}

/*
* StartRound
* Activates the starting sequence for the current round
*/
void AArenaGrid::StartRound()
{
	CalculateTilePositions();
}

/*
* EndRound
* Activates the ending sequence for the current round
*/
void AArenaGrid::EndRound()
{
	// Reset the center tile to it's max height
	FloorPieces[0]->SetActorLocation(FVector(FloorPieces[0]->GetActorLocation().X, FloorPieces[0]->GetActorLocation().Y, MaxHeight));

	// Reset all other tiles to the min height
	for (int i = 1; i < FloorPieces.Num(); i++)
	{
		FloorPieces[i]->SetActorLocation(FVector(FloorPieces[i]->GetActorLocation().X, FloorPieces[i]->GetActorLocation().Y, MinHeight));
	}

	// Clear floor heights
	FloorHeights.Empty();
}

/*
* SaveState
* Save the current hex grid state to the saved state array at the specified index.
* If no index is given will append to the end of the array
*	-Param index (default -1): The index to save at. Default or invalid appends
*	-Param freshState: This governs whether current editor data is saved or ignored
*	-Return result: The index at which the new save state was stored
*/
int AArenaGrid::SaveState(int index, bool freshState)
{
	FSaveState tmp;
	int i, result = 0;

	// If saving current editor data update the stored data with the data from actors in the editor
	if (!freshState)
		for (i = 0; i < FloorPieces.Num(); ++i)
		{
			if (FloorPieces[i])
				FloorHeights[i] = FloorPieces[i]->GetActorLocation().Z;
		}

	tmp = FSaveState(FloorHeights);

	if (SavedStates.IsValidIndex(index))
	{
		//result = SavedStates.Insert(tmp, index);
		SavedStates[index] = tmp;
	}
	if (index == -1)
	{
		result = SavedStates.Add(tmp);
	}

	return result;
}

/*
* Generate Heights
* Calls the calculate tile positions protected function in order to randomize heights
*/
void AArenaGrid::GenerateHeights()
{
	CalculateTilePositions();
}

void AArenaGrid::EraseHeightState(int index)
{
	//SavedStates.Empty();
	if (SavedStates.IsValidIndex(index))
	{
		SavedStates.RemoveAt(index);
	}
}

void AArenaGrid::LoadSaveState(int index, FVector origin, int radius, float padding)
{
	if (SavedStates.IsValidIndex(index))
	{
		// Set heights from saved state
		FloorHeights.Empty();
		FloorHeights = SavedStates[index].mHeights;

		// Initialize hex grid data
		InitHexGrid(radius);

		HexCell center = HexCell(0.0f, 0.0f, 0.0f);

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
				currentCell.SetID(i);

				// Calculate tile offset. See https://www.redblobgames.com/grids/hexagons/ (Hex to pixel section) for more info on coordinate conversion
				float xPos = padding * (FMath::Sqrt(3) * currentCell.GetQ() + FMath::Sqrt(3) / 2 * currentCell.GetR());
				float yPos = padding * (3.0f / 2.0f * currentCell.GetR());

				FVector tileOffset;
				if (currentCell.GetQ() == 0 && currentCell.GetR() == 0)
				{
					tileOffset = FVector(xPos, yPos, MaxHeight);
					center = currentCell;
				}
				else
				{
					// Set the height from the stored data in the tile offset
					tileOffset = FVector(xPos, yPos, GetTransform().GetLocation().Z) * padding;
					tileOffset.Z += FloorHeights[i];
				}

				// Spawn new tile
				FRotator rot = this->GetActorRotation();
				AActor* floorPiece = GetWorld()->SpawnActor<AActor>(FloorPieceActor, spawnLoc + tileOffset, rot, spawnParams);

				// Child new floor piece to the grid object
				FAttachmentTransformRules attachRules = FAttachmentTransformRules::KeepWorldTransform;
				floorPiece->AttachToActor(this, attachRules);

				// Set the correct rotation of the floor piece
				rot = FRotator(0.0f, 30.0f, 0.0f);
				floorPiece->AddActorLocalRotation(rot);

				// Add the floor piece to floor piece array
				FloorPieces.Add(floorPiece);
			}
		}

		CalculateRing(center, radius);
	}
	else
	{
		SpawnFloor(origin, radius, padding);
	}
}

// Called when the game starts or when spawned
void AArenaGrid::BeginPlay()
{
	Super::BeginPlay();
}

/*
* CalculateTilePositions
* Calculates the position of each tile using simplex noise
*/
void AArenaGrid::CalculateTilePositions()
{
	// Clear previous heights
	FloorHeights.Empty();

	for (int i = 0; i < FloorPieces.Num(); i++)
	{
		// Generates a float from 2D Perlin noise  
		float height = FMath::PerlinNoise2D(FVector2D(FloorPieces[i]->GetActorLocation().X, FloorPieces[i]->GetActorLocation().Y));
		height += MinHeight + 1;
		height *= (MaxHeight - MinHeight);
		//height = FMath::Clamp(height, MinHeight, MaxHeight);

 		FloorHeights.Add(height);
	}
}
/*
* Calculate Ring
* Calculates a ring around a tile with a given radius
*	-Param center: the center tile of the ring
*	-Param radius: the radius of the ring (in tiles)
* See https://www.redblobgames.com/grids/hexagons/ (Rings Section) for more info
*/
void AArenaGrid::CalculateRing(HexCell center, int radius)
{	
	// Calculate the location of the first cell in the ring
	HexCell currentCell = AddHex(center, MultiplyHex(GetHexDirection(4), radius));

	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < radius; j++)
		{
			// Get the neighbor of this cell
			Cells.Add(currentCell);
			currentCell = GetNeighbor(currentCell, i);
		}
	}
}

// Called every frame
void AArenaGrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

