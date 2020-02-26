// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "PiratePursuitGameMode.h"
#include "PiratePursuitCharacter.h"
#include "UObject/ConstructorHelpers.h"

APiratePursuitGameMode::APiratePursuitGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/PiratePursuit/Blueprints/BP_PirateCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
