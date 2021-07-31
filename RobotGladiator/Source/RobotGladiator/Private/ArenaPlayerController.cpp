// Fill out your copyright notice in the Description page of Project Settings.


#include "ArenaPlayerController.h"

void AArenaPlayerController::Spectate()
{
	ChangeState(NAME_Spectating);
	ClientGotoState(NAME_Spectating);
	ServerViewNextPlayer();
}
