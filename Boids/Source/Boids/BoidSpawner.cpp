#include "BoidSpawner.h"
#include "Engine/World.h"
#include "Kismet/KismetMathLibrary.h"

DEFINE_LOG_CATEGORY(BoidSpawnerLog);

ABoidSpawner::ABoidSpawner() {
	PrimaryActorTick.bCanEverTick = false;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(FName("SceneComponent"));
	SetRootComponent(SceneComponent);
}

void ABoidSpawner::BeginPlay() {
	Super::BeginPlay();

	if (!Active) {
		return;
	}

	if (ClassToSpawn) {
		for (int index = 0; index < SpawnCount; index++) {
			FVector SpawnLocation = UKismetMathLibrary::RandomUnitVector() * SpawnRadius + GetActorLocation();
			GetWorld()->SpawnActor<ABoid>(ClassToSpawn, SpawnLocation, RandomSpawnRotation());
		}
	} else {
		UE_LOG(BoidSpawnerLog, Error, TEXT("Class of objects to spawn is missing"));
	}
}

/* also see UKismetMathLibrary::RandomRotator */
FRotator ABoidSpawner::RandomSpawnRotation() {
	FRotator RRot;
	RRot.Yaw = RandomYaw ? FMath::FRand() * 360.f : 0;
	RRot.Pitch = RandomPitch ? FMath::FRand() * 360.f : 0;
	RRot.Roll = RandomRoll ? FMath::FRand() * 360.f : 0;
	return RRot;
}
