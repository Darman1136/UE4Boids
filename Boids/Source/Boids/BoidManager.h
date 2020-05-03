#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Boid.h"
#include "HAL/Runnable.h"
#include "Multithreading/BoidCalculationWorker.h"
#include "BoidManager.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(BoidManagerLog, Log, All);

UCLASS()
class BOIDS_API ABoidManager : public AActor {
	GENERATED_BODY()

public:
	ABoidManager();

public:
	virtual void Tick(float DeltaTime) override;

	virtual void AddManagedBoid(ABoid* Boid);

	virtual void RemoveManagedBoid(ABoid* Boid);

	bool GetBoidsFollowTarget() const { return BoidsFollowTarget; }

	float GetSeparationWeight() const { return SeparationWeight; }

	float GetAlignmentWeight() const { return AlignmentWeight; }

	float GetCohesionWeight() const { return CohesionWeight; }

	float GetTargetWeight() const { return TargetWeight; }

protected:
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

protected:
	// Settings
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
		bool Active = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
		int32 AmountOfThreads = 5;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
		int32 BoidUpdatePerTick = 20;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
		bool BoidsFollowTarget = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
		float SeparationWeight = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
		float AlignmentWeight = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
		float CohesionWeight = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
		float TargetWeight = 2.f;

	// Info
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Info")
		TArray<ABoid*> ManagedBoids;

private:
	int32 TickUpdateCounter = 0;

	// add boid to the thread with the least managed boids
	void AddBoidToThread(ABoid* Boid);

private:
	// is manager initialized? (are threads/runnables created)
	bool InitializationDone = false;

	// list of boids to be distributed to worker threads/runnables
	TArray<ABoid*> BoidsToDistributeToThreads;

	TArray<FBoidCalculationWorker*> Runnables;
};
