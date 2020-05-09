#include "BoidCalculationWorker.h"
#include "Boid.h"
#include "HAL/RunnableThread.h"
#include "Runtime/Core/Public/Async/Async.h"

DEFINE_LOG_CATEGORY(BoidCalculationWorkerLog);

FBoidCalculationWorker::FBoidCalculationWorker() : StopTaskCounter(0) {
	LocalThread = FRunnableThread::Create(this, TEXT("FBoidCalculationWorker"));
}

FBoidCalculationWorker::~FBoidCalculationWorker() {
	delete LocalThread;
	LocalThread = NULL;
	UE_LOG(BoidCalculationWorkerLog, Log, TEXT("Destruction"));
}

bool FBoidCalculationWorker::Init() {
	UE_LOG(BoidCalculationWorkerLog, Log, TEXT("Init"));
	return true;
}

uint32 FBoidCalculationWorker::Run() {
	while (StopTaskCounter.GetValue() == 0) {
		for (ABoid* Boid : ManagedBoids) {
			Boid->CalculateBoidRotation();
		}
		FPlatformProcess::Sleep(0.01);
	}
	return 0;
}

void FBoidCalculationWorker::Stop() {
	StopTaskCounter.Increment();
}

void FBoidCalculationWorker::EnsureCompletion() {
	Stop();
	if (LocalThread) {
		LocalThread->WaitForCompletion();
	}
}

void FBoidCalculationWorker::AddBoid(ABoid * Boid) {
	ManagedBoids.Add(Boid);
}
