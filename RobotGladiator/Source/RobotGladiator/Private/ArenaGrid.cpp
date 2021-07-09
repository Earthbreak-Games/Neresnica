/**
 * @file ArenaGrid.cpp
 * @brief Defines functionality of the Arena Grid class which is responsible for generating and managing a hexagonal grid
 * @dependencies UnrealMathUtility.h, KismetMathLibrary.h, CoreMinimal.h, ArenaGrid.h, HexCell.h
 *
 * @author Ethan Heil
 * @author Henry Chronowski - State Saving/Editing
 * @credits 
 *	https://www.redblobgames.com/grids/hexagons/
 **/

#include "ArenaGrid.h"

#define ModifierIDs FSaveState::ModifierIDs

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

	// Seed the random stream
	mRand = FRandomStream();
	mRand.GenerateNewSeed();
}

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
				//tileOffset = FVector(xPos, yPos, MaxHeight);
				center = currentCell;
			}
			//else
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

void AArenaGrid::InitHexGrid(int radius)
{
	// Create center tile
	HexCell center = HexCell(0.0f, 0.0f, 0.0f);
	Cells.Add(center);
	
	// Create tiles in a ring around the center
	for(int i = 0; i <= radius; i++)
		CalculateRing(center, i);

}

void AArenaGrid::StartRound()
{
	CalculateTilePositions();
}

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


void AArenaGrid::SetupLobbyOrientation(int numTiles)
{
	for (int i = 0; i < numTiles; i++)
	{
		FloorPieces[i]->SetActorLocation(FVector(FloorPieces[i]->GetActorLocation().X, FloorPieces[i]->GetActorLocation().Y, -MaxHeight));
	}
}

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

	// Store the current float floor heights as a save state
	tmp = FSaveState(FloorHeights);

	// If the index is of a pre-existing save overwrite that save, otherwise create a new one
	if (SavedStates.IsValidIndex(index))
	{
		SavedStates[index] = tmp;
	}
	else if (index == -1)
	{
		result = SavedStates.Add(tmp);
	}

	// Return the index of the state that was just stored
	return result;
}

void AArenaGrid::GenerateArena(float chance, float scale)
{
	scale *= 0.001;
	CalculateTilePositions(scale);
	CalculateTileModifiers(chance);
}

void AArenaGrid::EraseHeightState(int index)
{
	if (SavedStates.IsValidIndex(index))
	{
		SavedStates.RemoveAt(index);
	}
}

void AArenaGrid::EditorLoadSaveState(int index, FVector origin, int radius, float padding)
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
	// If an invalid index is entered generate the default arena
	else
	{
		SpawnFloor(origin, radius, padding);
	}
}

void AArenaGrid::LoadSaveState(UPARAM(ref) int&index, float scale)
{
	// Load the next saved state if one exists, if not generate a new arena
	if (SavedStates.IsValidIndex(index))
	{
		DEBUGMESSAGE("Loading Saved State %i", index);
		FloorHeights.Empty();
		// TODO: random selection
		FloorHeights = SavedStates[index].mHeights;
		index++;
	}
	else
	{
		DEBUGMESSAGE("Generating new arena")
		GenerateArena(scale);
		index++;
	}
}

// Called when the game starts or when spawned
void AArenaGrid::BeginPlay()
{
	Super::BeginPlay();
}

void AArenaGrid::CalculateTilePositions(float scale)
{
	// Clear previous heights
	FloorHeights.Empty();

	// A time-based pseudo-random seed for the noise sample
	float seed = mRand.FRand();

	// Generate a new height for each hex cell
	for (int i = 0; i < FloorPieces.Num(); i++)
	{
		// Generates a float from 2D Perlin noise using the world location of the hex cells as input
		float height = FMath::PerlinNoise2D(FVector2D(seed * scale * FloorPieces[i]->GetActorLocation().X,
													  seed * scale * FloorPieces[i]->GetActorLocation().Y));

		// Translate the height from a [-1,1] scale to a [0,2] scale
		height += 1.0f;

		// Scale the height to a [0,1] and then further scale to a [0,Range] 
		height *= 0.5 * (MaxHeight - MinHeight);

		// Translate the height to a [MinHeight,MaxHeight] scale
		height += MinHeight;

		// Add the new height to the array
 		FloorHeights.Add(height);
	}
}

void AArenaGrid::CalculateTileModifiers(float chance)
{
	// Clear previous modifiers and split remaining percentage into each modifier
	FloorModifiers.Empty();
	float ratio = (100.0f - chance) / (ModifierIDs::NUM_MODIFIERS - 1);

	// Generate new modifiers for each hex cell
	float modifier;
	for (int i = 0; i < FloorPieces.Num(); i++)
	{
		// Generate a random number from 0 to 100 and set the modifier
		modifier = mRand.FRandRange(0.0f, 100.0f);
		if (modifier > chance)
		{
			modifier -= chance;
			modifier = fmod(modifier, ratio) + 1;
		}

		// Store the generated modifier in the save state
		SavedStates[i].mModifiers = StaticCast<int>(modifier);
	}
}

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

