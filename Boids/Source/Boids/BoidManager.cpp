#include "BoidManager.h"
#include "Libraries/ArrayLibrary.h"

DEFINE_LOG_CATEGORY(BoidManagerLog);

ABoidManager::ABoidManager() {
	PrimaryActorTick.bCanEverTick = true;
}

void ABoidManager::BeginPlay() {
	Super::BeginPlay();
	for (int32 i = 0; i < AmountOfThreads; i++) {
		Runnables.Add(new FBoidCalculationWorker());
	}
	InitializationDone = true;
}

void ABoidManager::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Super::EndPlay(EndPlayReason);
	for (FBoidCalculationWorker* Runnable : Runnables) {
		Runnable->EnsureCompletion();
		delete Runnable;
	}
}

void ABoidManager::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (!Active) {
		return;
	}


	if (InitializationDone && !ArrayLibrary::IsEmpty<ABoid*>(BoidsToDistributeToThreads)) {
		for (ABoid* Boid : BoidsToDistributeToThreads) {
			AddBoidToThread(Boid);
		}
		BoidsToDistributeToThreads.Empty();
	}

	if (!ArrayLibrary::IsEmpty<ABoid*>(ManagedBoids)) {
		// Move boids forward each tick
		for (ABoid* Boid : ManagedBoids) {
			Boid->UpdateBoidRotation(DeltaTime);
			Boid->CalculateBoidPosition(DeltaTime);
			Boid->UpdateBoidPosition();
		}
	}
}

void ABoidManager::AddManagedBoid(ABoid* Boid) {
	ManagedBoids.AddUnique(Boid);
	// Add Boid to list to be distributed to threads
	BoidsToDistributeToThreads.Add(Boid);
	UE_LOG(BoidManagerLog, Log, TEXT("Start managing %s"), *Boid->GetName());
}

void ABoidManager::RemoveManagedBoid(ABoid* Boid) {
	ManagedBoids.Remove(Boid);
	UE_LOG(BoidManagerLog, Log, TEXT("Stop managing %s"), *Boid->GetName());
}

void ABoidManager::AddBoidToThread(ABoid * Boid) {
	int32 LeastManagedBoids = INT_MAX;
	FBoidCalculationWorker* RunnableWithLeastBoids = nullptr;
	for (FBoidCalculationWorker* Runnable : Runnables) {
		int32 ManageBoidsCount = Runnable->ManagedBoidsCount();
		if (ManageBoidsCount < LeastManagedBoids) {
			LeastManagedBoids = ManageBoidsCount;
			RunnableWithLeastBoids = Runnable;
		}
	}
	if (RunnableWithLeastBoids) {
		RunnableWithLeastBoids->AddBoid(Boid);
	}
}
