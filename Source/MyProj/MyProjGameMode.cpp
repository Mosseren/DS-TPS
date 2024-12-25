// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyProjGameMode.h"
#include "MyProjCharacter.h"
#include "UObject/ConstructorHelpers.h"

AMyProjGameMode::AMyProjGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
