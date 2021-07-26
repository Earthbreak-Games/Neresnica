// Fill out your copyright notice in the Description page of Project Settings.


#include "GruntBase.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/OutputDevice.h"

AGruntBase::AGruntBase()
{
	SetActorTickEnabled(true);
	mHealth = 10.0f;
	mIsAttacking = false;
	mMeleeRange = 0.0f;
	mDistanceToTarget = 9999;
}


void AGruntBase::BeginPlay()
{
	Super::BeginPlay();

	if (mpTarget == nullptr)
	{
		TArray<AActor*> foundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), mClasstoFind, foundActors);

		mpTarget = GetClosestPlayer(foundActors);
	}
}

void AGruntBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TArray<AActor*> foundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), mClasstoFind, foundActors);
	AActor* target = GetClosestPlayer(foundActors);

	if (mpTarget == nullptr || (mpTarget != nullptr && target != mpTarget))
	{
		mpTarget = target;
	}
	else if (mpTarget != nullptr)
	{
		FVector dir = mpTarget->GetActorLocation() - this->GetActorLocation();
		mDistanceToTarget = dir.Size();
	}

	if (!mIsAttacking && mpTarget != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Attacking"));
		if (mDistanceToTarget <= mMeleeRange)
		{
			MeleeAttack();
			mIsAttacking = true;
		}
	}
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
					mDistanceToTarget = distance;
				}
			}
		}
	}
	return closestPlayer;
}