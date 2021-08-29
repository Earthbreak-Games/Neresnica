// Fill out your copyright notice in the Description page of Project Settings.


#include "MyNavLinkProxy.h"

AMyNavLinkProxy::AMyNavLinkProxy(){
	GetSmartLinkComp()->GetLinkId();

	

}


void AMyNavLinkProxy::BeginPlay() {
	Super::BeginPlay();

	base_ptr = &(ANavLinkProxy::PointLinks); // listen i know its ugly but it works
}



void AMyNavLinkProxy::Set_Jump_Points(FVector left, FVector right) {

	auto link_data = base_ptr->GetData();
	
	
	link_data->Left = left;
	link_data->Right = right;

	GetSmartLinkComp()->SetLinkData(link_data->Right, link_data->Left, ENavLinkDirection::BothWays);
	SetSmartLinkEnabled(true);

}

