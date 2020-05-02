#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Boid.h"
#include "BoidSpawner.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(BoidSpawnerLog, Log, All);

UCLASS()
class BOIDS_API ABoidSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	ABoidSpawner();

protected:
	virtual void BeginPlay() override;

protected:
	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class USceneComponent* SceneComponent;

	// Settings
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
		bool Active = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
		TSubclassOf<ABoid> ClassToSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
		int32 SpawnCount = 30;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
		float SpawnRadius = 500.f;
};
