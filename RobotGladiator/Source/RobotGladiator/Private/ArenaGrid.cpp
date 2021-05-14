// Fill out your copyright notice in the Description page of Project Settings.

#include "ArenaGrid.h"

// Sets default values
AArenaGrid::AArenaGrid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AArenaGrid::SpawnFloor(FVector origin, int size, int radius, int padding)
{
	InitHexGrid(radius);
	for (int i = 0; i < radius; i++)
	{
		FActorSpawnParameters spawnParams;
		spawnParams.Owner = this;
		spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		FVector spawnLoc = origin;

		if (FloorPieceActor)
		{
			AActor* floorPiece = GetWorld()->SpawnActor<AActor>(FloorPieceActor, spawnLoc, this->GetActorRotation(), spawnParams);
		}
	}
}

void AArenaGrid::InitHexGrid(int radius)
{
	for (int col = -radius; col < radius; col++)
	{
		int rowMax = FMath::Max(-radius, -col - radius);
		int rowMin = FMath::Min(radius, -col + radius);
		for (int row = rowMin; row < rowMax; row++)
		{
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

