#include "BoidPlayerController.h"

ABoidPlayerController::ABoidPlayerController() {
	bShowMouseCursor = true;
}

void ABoidPlayerController::BeginPlay() {
	Super::BeginPlay();
	SetInputMode(FInputModeGameAndUI());
}
