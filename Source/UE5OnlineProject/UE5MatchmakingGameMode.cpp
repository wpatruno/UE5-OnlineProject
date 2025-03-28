// Copyright Epic Games, Inc. All Rights Reserved.

#include "UE5MatchmakingGameMode.h"
#include "UE5MatchmakingCharacter.h"
#include "UObject/ConstructorHelpers.h"

AUE5MatchmakingGameMode::AUE5MatchmakingGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
