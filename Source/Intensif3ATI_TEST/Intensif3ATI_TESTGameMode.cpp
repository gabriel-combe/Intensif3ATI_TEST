// Copyright Epic Games, Inc. All Rights Reserved.

#include "Intensif3ATI_TESTGameMode.h"
#include "Intensif3ATI_TESTPlayerController.h"
#include "Intensif3ATI_TESTCharacter.h"
#include "UObject/ConstructorHelpers.h"

AIntensif3ATI_TESTGameMode::AIntensif3ATI_TESTGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = AIntensif3ATI_TESTPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	// set default controller to our Blueprinted controller
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownPlayerController"));
	if(PlayerControllerBPClass.Class != NULL)
	{
		PlayerControllerClass = PlayerControllerBPClass.Class;
	}
}