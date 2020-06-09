#include "Boid.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Libraries/ArrayLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "BoidManager.h"
#include "BoidTarget.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

DEFINE_LOG_CATEGORY(BoidLog);

ABoid::ABoid() {
	PrimaryActorTick.bCanEverTick = false;

	BoxCollisionComponent = CreateDefaultSubobject<UBoxComponent>(FName("SceneComponent"));
	BoxCollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BoxCollisionComponent->SetBoxExtent(FVector(16.f));
	SetRootComponent(BoxCollisionComponent);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Mesh"));
	Mesh->SetCollisionProfileName(FName("Boid"));
	Mesh->SetCollisionProfileName(FName("Boid"));
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Mesh->SetupAttachment(BoxCollisionComponent);

	ArrowComponent = CreateDefaultSubobject<UArrowComponent>(FName("ArrowComponent"));
	ArrowComponent->SetupAttachment(BoxCollisionComponent);

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(FName("SpringArmComponent"));
	SpringArmComponent->TargetArmLength = 120.f;
	SpringArmComponent->SocketOffset = FVector(0.f, 0.f, 20.f);
	SpringArmComponent->bDoCollisionTest = false;
	SpringArmComponent->bEnableCameraLag = true;
	SpringArmComponent->bEnableCameraRotationLag = true;
	SpringArmComponent->SetupAttachment(BoxCollisionComponent);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(FName("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);
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
	if (!BoidTarget) {
		UE_LOG(BoidLog, Error, TEXT("Failed to find BoidTarget in scene"));
	}
}

void ABoid::BeginDestroy() {
	if (Manager) {
		Manager->RemoveManagedBoid(this);
	}
	Super::BeginDestroy();
}

void ABoid::CalculateBoidRotation() {
	TArray<ABoid*> CloseBoids = CalculateClosestBoids(AmountOfBoidsToObserve);

	FVector InterpolatedForwardVector = FVector::ZeroVector;

	FVector AlignmentVector = FVector::ZeroVector;
	FVector CohesionVector = FVector::ZeroVector;
	FVector SeparationVector = FVector::ZeroVector;
	FVector TargetVector = FVector::ZeroVector;
	if (!ArrayLibrary::IsEmpty<ABoid*>(CloseBoids)) {
		for (int index = 0; index < CloseBoids.Num(); index++) {
			ABoid* Boid = CloseBoids[index];
			CalculateAlignment(AlignmentVector, Boid);
			CalculateCohesion(CohesionVector, Boid);
			CalculateSeparation(SeparationVector, Boid);
		}
		AlignmentVector /= CloseBoids.Num();
		CohesionVector /= CloseBoids.Num();
		SeparationVector /= CloseBoids.Num();

		AlignmentVector.Normalize();
		CohesionVector.Normalize();
		SeparationVector.Normalize();
	}

	if (Manager->IsBoidsFollowTarget()) {
		TargetVector = CalculateTarget();
	}

	InterpolatedForwardVector += AlignmentVector * Manager->GetAlignmentWeight();
	InterpolatedForwardVector += CohesionVector * Manager->GetCohesionWeight();
	InterpolatedForwardVector += SeparationVector * Manager->GetSeparationWeight();
	InterpolatedForwardVector += TargetVector * Manager->GetTargetWeight();

	InterpolatedForwardVector *= TurnSpeed;

	InterpolatedForwardVector.Normalize();

	NextBoidRotation = UKismetMathLibrary::MakeRotFromX(InterpolatedForwardVector);
}

void ABoid::UpdateBoidRotation(float DeltaTime) {
	SetActorRotation(FMath::RInterpTo(GetActorRotation(), NextBoidRotation, DeltaTime, 1.f));
}

void ABoid::CalculateBoidPosition(float DeltaTime) {
	FVector SimpleForwardMovement = GetActorForwardVector();
	NextBoidWorldOffset = ForwardSpeed * DeltaTime * (SimpleForwardMovement);
}

void ABoid::UpdateBoidPosition() {
	AddActorWorldOffset(NextBoidWorldOffset);
}

void ABoid::CalculateSeparation(FVector &Separation, ABoid* Boid) {
	FVector Sub = GetActorLocation() - Boid->GetActorLocation();
	Separation += Sub * Sub.GetSafeNormal().Size();
}

void ABoid::CalculateAlignment(FVector &Alignment, ABoid* Boid) {
	Alignment += Boid->GetActorForwardVector();
}

void ABoid::CalculateCohesion(FVector &Cohesion, ABoid* Boid) {
	Cohesion += Boid->GetActorLocation();
}

FVector ABoid::CalculateTarget() {
	FVector Target = FVector::ZeroVector;
	if (BoidTarget) {
		Target = BoidTarget->GetActorLocation() - GetActorLocation();
		Target.Normalize();
	}
	return Target;
}

// Collision sphere seems to be way slower than calculating distance by hand
TArray<ABoid*> ABoid::CalculateClosestBoids(int32 Amount) {
	TArray<ABoid*> rc;

	if (Manager) {
		TArray<FOtherBoidInfo> OtherBoidInfos;
		for (ABoid* Other : Manager->GetManagedBoids()) {
			if (!Other || this == Other) {
				continue;
			}
			float OtherDistance = (Other->GetActorLocation() - GetActorLocation()).Size();
			if (OtherDistance < CloseBoidDistanceCutOff) {
				FOtherBoidInfo OtherInfo(Other, OtherDistance);
				OtherBoidInfos.Add(OtherInfo);
			}
		}
		// Sort by distance
		OtherBoidInfos.Sort([this](const FOtherBoidInfo& O1, const FOtherBoidInfo& O2) {
			return O1.Distance < O2.Distance;
			});

		for (int32 i = 0; i < Amount && i < OtherBoidInfos.Num(); i++) {
			rc.Add(OtherBoidInfos[i].Other);
		}
	}

	return rc;
}

