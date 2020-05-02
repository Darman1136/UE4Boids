#include "Boid.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Components/ArrowComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Libraries/ArrayLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "BoidManager.h"
#include "BoidTarget.h"

DEFINE_LOG_CATEGORY(BoidLog);

ABoid::ABoid() {
	PrimaryActorTick.bCanEverTick = false;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(FName("SceneComponent"));
	SetRootComponent(SceneComponent);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Mesh"));
	Mesh->SetCollisionProfileName(FName("Boid"));
	Mesh->SetupAttachment(SceneComponent);

	CloseBoidCollisionSphere = CreateDefaultSubobject<USphereComponent>(FName("CollisionCloseBoidCollisionSphere"));
	CloseBoidCollisionSphere->SetSphereRadius(126);
	CloseBoidCollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &ABoid::OnBoidBeginOverlap);
	CloseBoidCollisionSphere->OnComponentEndOverlap.AddDynamic(this, &ABoid::OnBoidEndOverlap);
	CloseBoidCollisionSphere->SetupAttachment(Mesh);

	ArrowComponent = CreateDefaultSubobject<UArrowComponent>(FName("ArrowComponent"));
	ArrowComponent->SetupAttachment(SceneComponent);
}

void ABoid::BeginPlay() {
	Super::BeginPlay();

	Manager = Cast<ABoidManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ABoidManager::StaticClass()));
	if (Manager) {
		Manager->AddManagedBoid(this);
	} else {
		UE_LOG(BoidLog, Error, TEXT("Failed to find BoidManager in scene"));
	}

	BoidTarget = Cast<ABoidTarget>(UGameplayStatics::GetActorOfClass(GetWorld(), ABoidTarget::StaticClass()));
	if (BoidTarget) {
		UE_LOG(BoidLog, Error, TEXT("Failed to find BoidTarget in scene"));
	}
}

void ABoid::BeginDestroy() {
	if (Manager) {
		Manager->RemoveManagedBoid(this);
	}
	Super::BeginDestroy();
}

void ABoid::CalculateBoidRotation(float DeltaTime) {
	FVector InterpolatedForwardVector = FVector::ZeroVector;

	InterpolatedForwardVector += CalculateAlignment() * Manager->GetSeparationWeight();
	InterpolatedForwardVector += CalculateCohesion()* Manager->GetCohesionWeight();
	InterpolatedForwardVector += CalculateSeparation()* Manager->GetSeparationWeight();
	if (Manager->GetBoidsFollowTarget()) {
		InterpolatedForwardVector += CalculateTarget()* Manager->GetTargetWeight();
	}

	InterpolatedForwardVector = FMath::VInterpTo(GetActorForwardVector(), InterpolatedForwardVector, DeltaTime, 1.f);
	InterpolatedForwardVector.Normalize();

	NextBoidRotation = UKismetMathLibrary::MakeRotFromX(InterpolatedForwardVector);
}

void ABoid::UpdateBoidRotation() {
	SetActorRotation(NextBoidRotation);
}

void ABoid::CalculateBoidPosition(float DeltaTime) {
	FVector SimpleForwardMovement = GetActorForwardVector();
	NextBoidWorldOffset = ForwardSpeed * DeltaTime * (SimpleForwardMovement);
}

void ABoid::UpdateBoidPosition() {
	AddActorWorldOffset(NextBoidWorldOffset);
}

FVector ABoid::CalculateSeparation() {
	FVector Separation = FVector::ZeroVector;
	if (!ArrayLibrary::IsEmpty<ABoid*>(CloseBoids)) {
		for (ABoid* Boid : CloseBoids) {
			FVector Sub = GetActorLocation() - Boid->GetActorLocation();
			Separation += Sub * Sub.GetSafeNormal().Size();
		}
		Separation /= CloseBoids.Num();
		Separation.Normalize();
	}
	return Separation;
}

FVector ABoid::CalculateAlignment() {
	FVector Alignment = FVector::ZeroVector;
	if (!ArrayLibrary::IsEmpty<ABoid*>(CloseBoids)) {
		for (ABoid* Boid : CloseBoids) {
			Alignment += Boid->GetActorForwardVector();
		}
		Alignment /= CloseBoids.Num();
		Alignment.Normalize();
	}
	return Alignment;
}

FVector ABoid::CalculateCohesion() {
	FVector Cohesion = FVector::ZeroVector;
	if (!ArrayLibrary::IsEmpty<ABoid*>(CloseBoids)) {
		for (ABoid* Boid : CloseBoids) {
			Cohesion += Boid->GetActorLocation();
		}
		Cohesion /= CloseBoids.Num();
		Cohesion.Normalize();
	}
	return Cohesion;
}

FVector ABoid::CalculateTarget() {
	FVector Target = FVector::ZeroVector;
	if (BoidTarget) {
		Target = BoidTarget->GetActorLocation() - GetActorLocation();
		Target.Normalize();
	}
	return Target;
}

void ABoid::OnBoidBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (OtherActor && OtherActor->IsA(ABoid::StaticClass())) {
		CloseBoids.AddUnique(Cast<ABoid>(OtherActor));
		UE_LOG(BoidLog, Warning, TEXT("Added %s"), *OtherActor->GetName());
	}
}

void ABoid::OnBoidEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	if (OtherActor && OtherActor->IsA(ABoid::StaticClass())) {
		CloseBoids.Remove(Cast<ABoid>(OtherActor));
		UE_LOG(BoidLog, Warning, TEXT("Removed %s"), *OtherActor->GetName());
	}
}
