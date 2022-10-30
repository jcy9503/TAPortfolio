// Copyright Epic Games, Inc. All Rights Reserved.

#include "LucasGameMode.h"
#include "LucasCharacter.h"
#include "UObject/ConstructorHelpers.h"

ALucasGameMode::ALucasGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
