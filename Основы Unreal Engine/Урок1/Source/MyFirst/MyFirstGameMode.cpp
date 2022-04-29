// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyFirstGameMode.h"
#include "MyFirstCharacter.h"
#include "UObject/ConstructorHelpers.h"

AMyFirstGameMode::AMyFirstGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
