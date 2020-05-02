#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Boid.h"
#include "BoidManager.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(BoidManagerLog, Log, All);

UCLASS()
class BOIDS_API ABoidManager : public AActor
{
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
	// Settings
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
		bool Active = true;

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

};
