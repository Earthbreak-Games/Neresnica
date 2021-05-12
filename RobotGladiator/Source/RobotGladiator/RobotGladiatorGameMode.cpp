// Copyright Epic Games, Inc. All Rights Reserved.

#include "RobotGladiatorGameMode.h"
#include "RobotGladiatorCharacter.h"
#include "UObject/ConstructorHelpers.h"

ARobotGladiatorGameMode::ARobotGladiatorGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
