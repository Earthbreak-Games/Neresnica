// Fill out your copyright notice in the Description page of Project Settings.


#include "GruntBase.h"


AGruntBase::AGruntBase()
{
	SetActorTickEnabled(true);

	mTimeLeftOnMeleeCoolDown = 2.0;
	mHealth = 10.0f;
}

AActor* AGruntBase::GetClosestPlayer(TArray<AActor*> Array)
{
	AActor* closestPlayer = nullptr;
	float distance = INT_MAX;


	//find the closes actor
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

				if (dir.SizeSquared() < distance * distance)
				{
					closestPlayer = actor;
					distance = dir.Size();
				}
			}
		}
	}

	return closestPlayer;
}