// Copyright Epic Games, Inc. All Rights Reserved.

#include "UniPurgeGameMode.h"
#include "UniPurgeCharacter.h"
#include "UObject/ConstructorHelpers.h"

AUniPurgeGameMode::AUniPurgeGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
