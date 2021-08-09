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
#include "DynamicNavLink.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Math/UnrealMathUtility.h"


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
	AddNavLinks();
	//DisplayLines();
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

	// Store the current float floor data as a save state
	tmp = FSaveState(FloorHeights, FloorModifiers);

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
	// Clear any remaining data from the previous level
	for (AActor* iter : Enemies)
	{
		iter->Destroy();
	}
	for (AActor* iter : Toppers)
	{
		iter->Destroy();
	}
	Enemies.Empty();
	Toppers.Empty();
	FloorHeights.Empty();

	// Load the next saved state if one exists, if not generate a new arena
	if (SavedStates.IsValidIndex(index))
	{
		// Get a reference to the saved state for easy access/readability
		DEBUGMESSAGE("Loading Saved State %i", index);
		FSaveState tmp = SavedStates[index];

		// Load tile heights									// TODO: random selection
		FloorHeights = tmp.mHeights;

		// Act on saved modifiers
		for (int i = 0; i < tmp.mModifiers.Num(); i++)
		{
			switch (tmp.mModifiers[i])
			{
			case ModifierIDs::TOPPER:		// Spawn Toppers
			{
				// Init spawn parameters
				FActorSpawnParameters spawnParams;
				spawnParams.Owner = this;
				spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

				// Set spawn location from the floorpeice currently at the location
				FVector loc = FloorPieces[i]->GetActorLocation();										// Like the other one this needs to be revised to deal with the movement issue
				loc.Z = FloorHeights[i] + 15.0f;	// Find a programmatic way to determine this

				// Check if actor to spawn is valid
				if (Topper)
				{
					// Spawn new tile
					FRotator rot = this->GetActorRotation();
					AActor* topper = GetWorld()->SpawnActor<AActor>(FloorPieceActor, loc, rot, spawnParams);

					// Child new floor piece to the grid object
					FAttachmentTransformRules attachRules = FAttachmentTransformRules::KeepWorldTransform;
					topper->AttachToActor(this, attachRules);

					// Set the correct rotation of the floor piece
					rot = FRotator(0.0f, 30.0f, 0.0f);
					topper->AddActorLocalRotation(rot);

					// Add the floor piece to floor piece array
					Toppers.Add(topper);
				}
				break;
			}
			case ModifierIDs::GLADIATOR:	// Spawn gladiators
			{
				// Init spawn parameters
				FActorSpawnParameters spawnParams;
				spawnParams.Owner = this;
				spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

				// Set spawn transform
				FVector loc = FloorPieces[i]->GetActorLocation();
				loc.Z = FloorHeights[i] + 1510.0f;	// Find a programmatic way to determine this
				FRotator rot = this->GetActorRotation();

				// Check if actor to spawn is valid
				if (Gladiator)
				{
					// Spawn new enemy
					AActor* enemy = GetWorld()->SpawnActor<AActor>(Gladiator, loc, rot, spawnParams);

					// Child new enemy to the grid object
					FAttachmentTransformRules attachRules = FAttachmentTransformRules::KeepWorldTransform;
					enemy->AttachToActor(this, attachRules);

					// Add the enemy to the array
					Enemies.Add(enemy);
				}
				break;
			}
			default:
				break;
			}			
		}

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
			modifier = StaticCast<int>(modifier / ratio) + 1;
		}
		else
		{
			modifier = ModifierIDs::NONE;
		}

		// Store the generated modifier in the save state
		FloorModifiers.Add(modifier);
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


void AArenaGrid::AddNavLinks()
{
	TArray<AActor*> links;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADynamicNavLink::StaticClass(), links);
	links.Empty();

	TArray<AActor*> cells;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ArenaCellClass, cells);								//FIX!!!!!!!!!!!!!!!!!!!!!!

	for (AActor* cell : cells)
	{
		

		TArray<AActor*> neighbors = GetNeighbors(cell);

		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString::Printf(TEXT("Vector Length: %i"), neighbors.Num()));

		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString::Printf(TEXT("Starting to place links")));

		for (AActor* neighbor : neighbors)
		{
			if (cell->GetActorLocation().Z - neighbor->GetActorLocation().Z > FMath::Abs(MaxHeightDifference))
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString::Printf(TEXT("Spawning link!")));
				SpawnNavLink(cell, neighbor);
			}
		}

	}


}
	

TArray<AActor*> AArenaGrid::GetNeighbors(AActor* cell)
{

	TArray<AActor*> neighbors;


	FVector hexDirs[6] = {};

	for (int i = 0; i < 6; i++)
	{

		FMath::SinCos(&hexDirs[i].X, &hexDirs[i].Y, ((1.0 + 2.0 * i) / 12.0) * 2.0 * PI);
		hexDirs[i].Z = 0;

		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString::Printf(TEXT("Vector: %s"), *hexDirs[i].ToString()));
	}

	TArray<TEnumAsByte<enum EObjectTypeQuery> > objectTypesToFind;
	objectTypesToFind.Add(EObjectTypeQuery::ObjectTypeQuery2);
	//ignore self
	TArray<AActor*> actorsToIgnore;
	FHitResult outHit;

	FVector start = cell->GetActorLocation() + LineTraceOffset;

	for (int i = 0; i < 6; i++)
	{
		UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), start, start + hexDirs[i] * TraceDistance, objectTypesToFind, false, actorsToIgnore, EDrawDebugTrace::Type::ForDuration, outHit, true, FLinearColor::Red, FLinearColor::Yellow, 50);
		if (outHit.GetActor() != nullptr)
		{
			neighbors.Add(outHit.GetActor());
		}
		
	}

	return neighbors;
}

void AArenaGrid::SpawnNavLink(AActor* cellOne, AActor* cellTwo)
{
	FActorSpawnParameters spawnParams;
	spawnParams.Owner = this;
	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;


	//AActor* topper = GetWorld()->SpawnActor<AActor>(FloorPieceActor, loc, rot, spawnParams);
	ADynamicNavLink* navLink = GetWorld()->SpawnActor<ADynamicNavLink>(ADynamicNavLink::StaticClass(), cellOne->GetActorLocation() + NAvLinkSpawnOffset, cellOne->GetActorRotation(), spawnParams);
	
	navLink->PointLinks.Add(FNavigationLink());
	navLink->PointLinks[0].Right = FVector(0, 0, 0);
	navLink->PointLinks[0].Left = (cellTwo->GetActorLocation() + NAvLinkSpawnOffset) - (cellOne->GetActorLocation() + NAvLinkSpawnOffset);
	navLink->PointLinks[0].Direction = ENavLinkDirection::Type::RightToLeft;
	navLink->SetAndEnableSmartLink();

	navLink->PointLinks.Empty();
	
}

void AArenaGrid::DisplayLines()
{
	TArray<AActor*> cells;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ArenaCellClass, cells);

	FVector hexDirs[6] = {};

	for (int i = 0; i < 6; i++)
	{
		
		FMath::SinCos(&hexDirs[i].X, &hexDirs[i].Y, ((1.0 + 2.0 * i) / 12.0) * 2.0 * PI);
		hexDirs[i].Z = 0;

		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString::Printf(TEXT("Vector: %s"), *hexDirs[i].ToString()));
	}

	for (AActor* cell : cells)
	{

		TArray<TEnumAsByte<enum EObjectTypeQuery> > objectTypesToFind;
		objectTypesToFind.Add(EObjectTypeQuery::ObjectTypeQuery1);
		//ignore self
		TArray<AActor*> actorsToIgnore;
		FHitResult outHit;

		FVector start = cell->GetActorLocation() + LineTraceOffset;

		for (int i = 0; i < 6; i++)
		{
			UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), start, start + hexDirs[i] * TraceDistance, objectTypesToFind, false, actorsToIgnore, EDrawDebugTrace::Type::ForDuration, outHit, true, FLinearColor::Red, FLinearColor::Yellow, 50);
			
		}

	}
	
}
