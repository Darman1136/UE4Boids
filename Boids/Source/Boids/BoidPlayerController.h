#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BoidPlayerController.generated.h"

UCLASS()
class BOIDS_API ABoidPlayerController : public APlayerController {
	GENERATED_BODY()

public:
	ABoidPlayerController();

protected:
	virtual void BeginPlay() override;
};
