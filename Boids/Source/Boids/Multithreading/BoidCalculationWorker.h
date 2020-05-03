#pragma once

#include "CoreMinimal.h"
#include "Boid.h"
#include "HAL/Runnable.h"

DECLARE_LOG_CATEGORY_EXTERN(BoidCalculationWorkerLog, Log, All);

class FBoidCalculationWorker : public FRunnable {

public:
	FBoidCalculationWorker();
	virtual ~FBoidCalculationWorker();

	virtual bool Init();
	virtual uint32 Run();
	virtual void Stop();

	void AddBoid(ABoid* Boid);
	int32 ManagedBoidsCount() const { return ManagedBoids.Num(); };

protected:
	FRunnableThread* LocalThread;

	TArray<ABoid*> ManagedBoids;

private:
	FThreadSafeCounter StopTaskCounter;

	double TimePrevious;
	double TimeCurrent;

};