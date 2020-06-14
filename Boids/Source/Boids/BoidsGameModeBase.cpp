#include "BoidsGameModeBase.h"
#include "BoidPlayerController.h"
#include "BoidDefaultPawn.h"

ABoidsGameModeBase::ABoidsGameModeBase() {
	PlayerControllerClass = ABoidPlayerController::StaticClass();
	DefaultPawnClass = ABoidDefaultPawn::StaticClass();
}