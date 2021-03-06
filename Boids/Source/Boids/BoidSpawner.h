#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Boid.h"
#include "BoidSpawner.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(BoidSpawnerLog, Log, All);

UCLASS(Config = Boids)
class BOIDS_API ABoidSpawner : public AActor {
	GENERATED_BODY()

public:
	ABoidSpawner();

protected:
	virtual void BeginPlay() override;

protected:
	// Components
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
		class USceneComponent* SceneComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
		class UBillboardComponent* BillboardComponent;

	// Settings
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
		bool Active = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
		TSubclassOf<ABoid> ClassToSpawn;

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Settings")
		int32 SpawnCount = 30;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
		float SpawnRadius = 500.f;

	// Spawn with random pitch
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Rotation")
		bool RandomPitch = true; 
	
	// Spawn with random yaw
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Rotation")
		bool RandomYaw = true; 
	
	// Spawn with random roll
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Rotation")
		bool RandomRoll = true;

private:
	// Creates a random spawn rotation. RandomPitch, RandomYaw and RandomRoll determine which axis are randomized.
	FRotator RandomSpawnRotation();
};
