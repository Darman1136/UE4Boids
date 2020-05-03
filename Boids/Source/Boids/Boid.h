#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Boid.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(BoidLog, Log, All);

UCLASS()
class BOIDS_API ABoid : public AActor {
	GENERATED_BODY()

public:
	ABoid();

	virtual void CalculateBoidRotation(float DeltaTime);

	virtual void CalculateBoidRotation(float DeltaTime, TArray<ABoid*> CloseBoidsLocal);

	virtual void UpdateBoidRotation();

	virtual void CalculateBoidPosition(float DeltaTime);

	virtual void UpdateBoidPosition();

	TArray<ABoid*> GetCloseBoids() const { return CloseBoids; };

protected:
	virtual void BeginPlay() override;

	virtual void BeginDestroy() override;

	virtual void CalculateSeparation(FVector &Spearation, ABoid* Boid);

	virtual void CalculateAlignment(FVector &Alignment, ABoid* Boid);

	virtual void CalculateCohesion(FVector &Cohesion, ABoid* Boid);

	virtual FVector CalculateTarget();

	UFUNCTION()
		virtual void OnBoidBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		virtual void OnBoidEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UBoxComponent* BoxCollisionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class USphereComponent* CloseBoidCollisionSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UArrowComponent* ArrowComponent;

	// Settings

	// The amount of boids to take into account when calculating the next position/rotation.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings", meta=(ClampMin = "1"))
		int32 AmountOfBoidsToObserve = 10;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
		float ForwardSpeed = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
		float TurnSpeed = 3.f;

	// Info

	// Array that tracks close boids. Close is defined by being inside the CloseBoidCollisionSphere.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Info")
		TArray<ABoid*> CloseBoids;

private:
	class ABoidManager* Manager;

	class ABoidTarget* BoidTarget;

	FRotator NextBoidRotation;
	FVector NextBoidWorldOffset;
};
