// Copyright Epic Games, Inc. All Rights Reserved.

#include "AutomatedTestsGameMode.h"
#include "AutomatedTestsHUD.h"
#include "AutomatedTestsCharacter.h"
#include "UObject/ConstructorHelpers.h"

AAutomatedTestsGameMode::AAutomatedTestsGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AAutomatedTestsHUD::StaticClass();
}
