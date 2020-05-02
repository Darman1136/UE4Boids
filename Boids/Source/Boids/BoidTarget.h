#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BoidTarget.generated.h"

UCLASS()
class BOIDS_API ABoidTarget : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABoidTarget();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UStaticMeshComponent* Mesh;
};
