#pragma once

#include "CoreMinimal.h"
#include "GameFramework/DefaultPawn.h"
#include "BoidDefaultPawn.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(BoidDefaultPawnLog, Log, All);

UCLASS()
class BOIDS_API ABoidDefaultPawn : public ADefaultPawn {
	GENERATED_BODY()

public:
	ABoidDefaultPawn();

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	virtual void SetupPlayerInputComponent(UInputComponent* InInputComponent) override;

	// Attaches/Detaches the target to the TargetAttachBoxComponent.
	void ToggleAttachTarget();

	// Attaches this pawn to a random boid
	void ToggleAttachToFollowRandom();

	void ChangeCameraDistanceUp();

	void ChangeCameraDistanceDown();

	// Changes distance of target to camera (+ => further away, - => closer)
	void ChangeCameraDistance(float Direction);

	void Quit();

private:
	void AttachTarget();

	void DetachTarget();

	void AttachToFollowRandom();

	void DetachFromFollowRandom();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
		FVector TargetAttachStartingOffset = FVector(5000.f, 0.f, 0.f);

	// Change in distance per mouse wheel turn "click" (up or down)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
		float TargetDistanceChangePerTurn = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
		float TargetDistanceClampMin = 200.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
		float TargetDistanceClampMax = 20000.f;

	UPROPERTY(VisibleAnywhere)
		class UBoxComponent* TargetAttachBoxComponent;

	UPROPERTY(VisibleAnywhere)
		class UCameraComponent* CameraComponent;

private:
	//Is target attached to pawn
	bool IsTargetAttached = false;

	//Is target attached to pawn
	bool IsAttachedToBoid = false;

	class ABoidTarget* BoidTarget;

	class APlayerController* PlayerController;
};
