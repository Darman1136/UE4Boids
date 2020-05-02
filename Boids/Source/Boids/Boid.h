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

	virtual void UpdateBoidRotation();

	virtual void CalculateBoidPosition(float DeltaTime);

	virtual void UpdateBoidPosition();

protected:
	virtual void BeginPlay() override;

	virtual void BeginDestroy() override;

	virtual FVector CalculateSeparation();

	virtual FVector CalculateAlignment();

	virtual FVector CalculateCohesion();

	virtual FVector CalculateTarget();

	UFUNCTION()
		virtual void OnBoidBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		virtual void OnBoidEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class USceneComponent* SceneComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class USphereComponent* CloseBoidCollisionSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UArrowComponent* ArrowComponent;

	// Settings
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
		float ForwardSpeed = 100.f;

	// Info
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Info")
		TArray<ABoid*> CloseBoids;

private:
	class ABoidManager* Manager;

	class ABoidTarget* BoidTarget;

	FRotator NextBoidRotation;
	FVector NextBoidWorldOffset;
};
