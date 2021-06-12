// Fill out your copyright notice in the Description page of Project Settings.


#include "GladiatorBase.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/OutputDevice.h"

AGladiatorBase::AGladiatorBase()
{
	SetActorTickEnabled(true);
}

void AGladiatorBase::BeginPlay()
{
	Super::BeginPlay();

	if (mpTarget == nullptr)
	{

		TArray<AActor*> foundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), mClasstoFind, foundActors);

		mpTarget = GetClosestPlayer(foundActors);


	}
}

AActor* AGladiatorBase::GetClosestPlayer(TArray<AActor*> Array)
{
	AActor* closestPlayer = nullptr;
	float distance = INT_MAX;

	//log all actors found for debugging
	for (AActor* actor : Array)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Names: %s"), *actor->GetName());
	}

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

void AGladiatorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TArray<AActor*> foundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), mClasstoFind, foundActors);
	AActor* target = GetClosestPlayer(foundActors);

	if (mpTarget == nullptr || (mpTarget != nullptr && target != mpTarget))
	{
		mpTarget = target;
	}


	FVector dir = mpTarget->GetActorLocation() - this->GetActorLocation();
	mDistanceToTarget = dir.Size();
	
	if ( !mIsOnCooldown && !mIsAttacking)
	{
	

		if (mDistanceToTarget <= mMeleeRange) 
		{
			MeleeAttack();
			mTimeLeftOnCoolDown = mMeleeCoolDown;
		}
		else
		{
			RangedAttack();
			mTimeLeftOnCoolDown = mRangedCoolDownTime;

		}

		
		//mIsOnCooldown = true;
	}
	else if (mIsOnCooldown)
	{
		mTimeLeftOnCoolDown -= DeltaTime;
		if (mTimeLeftOnCoolDown <= 0)
		{
			mTimeLeftOnCoolDown = 0;
			mIsOnCooldown = false;
		}
	}


	UE_LOG(LogTemp, Warning, TEXT("Cooldown: %s"), *(FString::SanitizeFloat(mTimeLeftOnCoolDown)));
}
