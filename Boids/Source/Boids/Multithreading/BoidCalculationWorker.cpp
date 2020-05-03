#include "BoidCalculationWorker.h"
#include "Boid.h"
#include "HAL/RunnableThread.h"
#include "Runtime/Core/Public/Async/Async.h"

DEFINE_LOG_CATEGORY(BoidCalculationWorkerLog);

FBoidCalculationWorker::FBoidCalculationWorker() : StopTaskCounter(0) {
	LocalThread = FRunnableThread::Create(this, TEXT("FBoidCalculationWorker"), 0, TPri_BelowNormal);
}

FBoidCalculationWorker::~FBoidCalculationWorker() {
	delete LocalThread;
	LocalThread = NULL;
	UE_LOG(BoidCalculationWorkerLog, Error, TEXT("Destruction"));
}

bool FBoidCalculationWorker::Init() {
	UE_LOG(BoidCalculationWorkerLog, Error, TEXT("Init"));
	return true;
}

uint32 FBoidCalculationWorker::Run() {

	while (StopTaskCounter.GetValue() == 0) {
		UE_LOG(BoidCalculationWorkerLog, Error, TEXT("Running %d"), ManagedBoids.Num());

		TimePrevious = TimeCurrent;
		TimeCurrent = FPlatformTime::Seconds();
		float DeltaTime = (float)(TimeCurrent - TimePrevious);

		for (ABoid* Boid : ManagedBoids) {
			TArray<ABoid*> CloseBoids = Boid->GetCloseBoids();
			UE_LOG(BoidCalculationWorkerLog, Error, TEXT("Close cnt %d"), CloseBoids.Num());
			//LocalBoid->CalculateBoidRotation(DeltaTime, CloseBoids);
		}

		FPlatformProcess::Sleep(0.01);
	}

	//UE_LOG(BoidCalculationWorkerLog, Error, TEXT("Run %s"), *LocalBoid->GetActorLocation().ToString());

	//while (StopTaskCounter.GetValue() == 0) {
	//	TimePrevious = TimeCurrent;
	//	TimeCurrent = FPlatformTime::Seconds();
	//	float DeltaTime = (float)(TimeCurrent - TimePrevious);


	//	//TArray<ABoid*> CloseBoids;
	//	//AsyncTask(ENamedThreads::GameThread,
	//	//	[this, &CloseBoids]() {  CloseBoids = LocalBoid->GetCloseBoids(); });
	//	//UE_LOG(BoidCalculationWorkerLog, Error, TEXT("Run %d"), CloseBoids.Num());
	//	UE_LOG(BoidCalculationWorkerLog, Error, TEXT("while %s"), *LocalBoid->GetName());
	//	FPlatformProcess::Sleep(0.01);
	//	//LocalBoid->CalculateBoidRotation(DeltaTime, CloseBoids);
	//}

	return 0;
}

void FBoidCalculationWorker::Stop() {
	StopTaskCounter.Increment();
}

void FBoidCalculationWorker::AddBoid(ABoid * Boid) {
	ManagedBoids.Add(Boid);
}
