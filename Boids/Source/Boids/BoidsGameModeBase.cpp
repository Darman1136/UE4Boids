#include "BoidsGameModeBase.h"
#include "BoidPlayerController.h"

ABoidsGameModeBase::ABoidsGameModeBase() {
	PlayerControllerClass = ABoidPlayerController::StaticClass();
}