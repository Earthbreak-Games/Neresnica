// Fill out your copyright notice in the Description page of Project Settings.

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

	UFUNCTION(BlueprintCallable)
	void SpawnFloor(FVector origin, int size, int radius, int padding);

	UFUNCTION(BlueprintCallable)
	void ClearFloor();

	void InitHexGrid(int radius);

public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AActor> FloorPieceActor;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<AActor*> FloorPieces;

	TArray<HexCell> Cells;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Radius;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
