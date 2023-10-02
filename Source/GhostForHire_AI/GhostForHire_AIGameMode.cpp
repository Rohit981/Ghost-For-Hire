// Copyright Epic Games, Inc. All Rights Reserved.

#include "GhostForHire_AIGameMode.h"
#include "GhostForHire_AICharacter.h"
#include "UObject/ConstructorHelpers.h"

AGhostForHire_AIGameMode::AGhostForHire_AIGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
