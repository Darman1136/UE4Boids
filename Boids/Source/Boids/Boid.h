#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Boid.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(BoidLog, Log, All);

struct FOtherBoidInfo {
	ABoid* Other;
	float Distance;

	FOtherBoidInfo(ABoid* Other_,
		float Distance_) : Other(Other_), Distance(Distance_) {
	}
};

UCLASS()
class BOIDS_API ABoid : public AActor {
	GENERATED_BODY()

public:
	ABoid();

	virtual void CalculateBoidRotation();

	virtual void UpdateBoidRotation(float DeltaTime);

	virtual void CalculateBoidPosition(float DeltaTime);

	virtual void UpdateBoidPosition();

	virtual TArray<ABoid*> CalculateClosestBoids(int32 Amount);

protected:
	virtual void BeginPlay() override;

	virtual void BeginDestroy() override;

	virtual void CalculateSeparation(FVector &Spearation, ABoid* Boid);

	virtual void CalculateAlignment(FVector &Alignment, ABoid* Boid);

	virtual void CalculateCohesion(FVector &Cohesion, ABoid* Boid);

	virtual FVector CalculateTarget();

protected:
	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UBoxComponent* BoxCollisionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UArrowComponent* ArrowComponent;

	// Settings

	// The amount of boids to take into account when calculating the next position/rotation.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings", meta = (ClampMin = "1"))
		int32 AmountOfBoidsToObserve = 10;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
		float ForwardSpeed = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
		float TurnSpeed = 3.f;

	// If a boid is within this distance it'll be used to calculate the next position/rotation
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
		float CloseBoidDistanceCutOff = 300.f;

private:
	class ABoidManager* Manager;

	class ABoidTarget* BoidTarget;

	FRotator NextBoidRotation;
	FVector NextBoidWorldOffset;
};
