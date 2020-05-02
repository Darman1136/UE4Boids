#include "BoidManager.h"
#include "Libraries/ArrayLibrary.h"

DEFINE_LOG_CATEGORY(BoidManagerLog);

ABoidManager::ABoidManager()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ABoidManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!Active) {
		return;
	}

	if (!ArrayLibrary::IsEmpty<ABoid*>(ManagedBoids)) {
		for (ABoid* Boid : ManagedBoids) {
			Boid->CalculateBoidRotation(DeltaTime);
		}
		for (ABoid* Boid : ManagedBoids) {
			Boid->UpdateBoidRotation();
		}
		for (ABoid* Boid : ManagedBoids) {
			Boid->CalculateBoidPosition(DeltaTime);
		}
		for (ABoid* Boid : ManagedBoids) {
			Boid->UpdateBoidPosition();
		}
	}
}

void ABoidManager::AddManagedBoid(ABoid* Boid) {
	ManagedBoids.AddUnique(Boid);
	UE_LOG(BoidManagerLog, Warning, TEXT("Start managing %s"), *Boid->GetName());
}

void ABoidManager::RemoveManagedBoid(ABoid* Boid) {
	ManagedBoids.Remove(Boid);
	UE_LOG(BoidManagerLog, Warning, TEXT("Stop managing %s"), *Boid->GetName());
}
