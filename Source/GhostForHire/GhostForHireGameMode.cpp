// Copyright Epic Games, Inc. All Rights Reserved.

#include "GhostForHireGameMode.h"
#include "GhostForHireCharacter.h"
#include "UObject/ConstructorHelpers.h"

AGhostForHireGameMode::AGhostForHireGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
