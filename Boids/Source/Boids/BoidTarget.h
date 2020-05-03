#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BoidTarget.generated.h"

UCLASS()
class BOIDS_API ABoidTarget : public AActor
{
	GENERATED_BODY()
	
public:	
	ABoidTarget();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UStaticMeshComponent* Mesh;
};
