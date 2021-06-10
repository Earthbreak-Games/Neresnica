// Fill out your copyright notice in the Description page of Project Settings.


#include "GladiatorBase.h"
#include "Kismet/GameplayStatics.h"

AGladiatorBase::AGladiatorBase()
{

}

void AGladiatorBase::BeginPlay()
{
	Super::BeginPlay();

	if (mpTarget == nullptr)
	{
		TSubclassOf<ARobotGladiatorCharacter> classtoFind;

		TArray<AActor*> foundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), classtoFind, foundActors);

		mpTarget = GetClosestPlayer(foundActors);


	}
}

AActor* AGladiatorBase::GetClosestPlayer(TArray<AActor*> Array)
{
	AActor* closestPlayer;
	float distance = 99999999999.0; //replace with max float


	for (AActor* actor : Array)
	{
		if (actor != this)
		{
			if (closestPlayer == nullptr)
			{
				closestPlayer = actor;
				FVector dir = closestPlayer->GetActorLocation() - this->GetActorLocation();
				distance = dir.Size();
			}
			else
			{
				FVector dir = actor->GetActorLocation() - this->GetActorLocation();

				if (dir.Size() < distance)//potentially rework to use sizesquared
				{
					closestPlayer = actor;
					distance = dir.Size();
				}
			}
		}
	}

	return closestPlayer;
}