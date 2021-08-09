// Fill out your copyright notice in the Description page of Project Settings.


#include "DynamicNavLink.h"


void ADynamicNavLink::SetAndEnableSmartLink()
{
	CopyEndPointsFromSimpleLinkToSmartLink();
	SetSmartLinkEnabled(true);
	bSmartLinkIsRelevant = true;
}

void ADynamicNavLink::ReceiveSmartLinkReached(AActor* Agent, const FVector& Destination)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString::Printf(TEXT("Smart Link Reached")));
}
