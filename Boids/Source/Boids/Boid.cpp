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

DEFINE_LOG_CATEGORY(BoidLog);

ABoid::ABoid() {
	PrimaryActorTick.bCanEverTick = false;

	BoxCollisionComponent = CreateDefaultSubobject<UBoxComponent>(FName("SceneComponent"));
	BoxCollisionComponent->SetBoxExtent(FVector(16.f));
	SetRootComponent(BoxCollisionComponent);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Mesh"));
	Mesh->SetCollisionProfileName(FName("Boid"));
	Mesh->SetupAttachment(BoxCollisionComponent);

	CloseBoidCollisionSphere = CreateDefaultSubobject<USphereComponent>(FName("CollisionCloseBoidCollisionSphere"));
	CloseBoidCollisionSphere->SetSphereRadius(126);
	CloseBoidCollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &ABoid::OnBoidBeginOverlap);
	CloseBoidCollisionSphere->OnComponentEndOverlap.AddDynamic(this, &ABoid::OnBoidEndOverlap);
	CloseBoidCollisionSphere->SetupAttachment(Mesh);

	ArrowComponent = CreateDefaultSubobject<UArrowComponent>(FName("ArrowComponent"));
	ArrowComponent->SetupAttachment(BoxCollisionComponent);
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

	FVector AlignmentVector = FVector::ZeroVector;
	FVector CohesionVector = FVector::ZeroVector;
	FVector SeparationVector = FVector::ZeroVector;
	FVector TargetVector = FVector::ZeroVector;
	if (!ArrayLibrary::IsEmpty<ABoid*>(CloseBoids)) {
		for (ABoid* Boid : CloseBoids) {
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

	if (Manager->GetBoidsFollowTarget()) {
		TargetVector = CalculateTarget();
	}

	InterpolatedForwardVector += AlignmentVector * Manager->GetAlignmentWeight();
	InterpolatedForwardVector += CohesionVector * Manager->GetCohesionWeight();
	InterpolatedForwardVector += SeparationVector * Manager->GetSeparationWeight();
	InterpolatedForwardVector += TargetVector * Manager->GetTargetWeight();

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
