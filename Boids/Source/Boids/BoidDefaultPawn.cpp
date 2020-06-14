#include "BoidDefaultPawn.h"
#include "Components/BoxComponent.h"
#include "Components/InputComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Boid.h"
#include "BoidTarget.h"
#include "Camera/CameraComponent.h"

DEFINE_LOG_CATEGORY(BoidDefaultPawnLog);

ABoidDefaultPawn::ABoidDefaultPawn() : Super() {
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(FName("CameraComponent"));
	CameraComponent->bUsePawnControlRotation = true;
	CameraComponent->SetupAttachment(RootComponent);

	TargetAttachBoxComponent = CreateDefaultSubobject<UBoxComponent>(FName("TargetAttachBoxComponent"));
	TargetAttachBoxComponent->SetRelativeLocation(TargetAttachStartingOffset);
	TargetAttachBoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	TargetAttachBoxComponent->SetCanEverAffectNavigation(false);
	TargetAttachBoxComponent->SetupAttachment(CameraComponent);
}

void ABoidDefaultPawn::BeginPlay() {
	Super::BeginPlay();

	UWorld* WorldContextObject = GetWorld();
	PlayerController = UGameplayStatics::GetPlayerController(WorldContextObject, 0);
	check(PlayerController);

	BoidTarget = Cast<ABoidTarget>(UGameplayStatics::GetActorOfClass(WorldContextObject, ABoidTarget::StaticClass()));
	check(BoidTarget);
}

void ABoidDefaultPawn::Tick(float DeltaSeconds) {
	Super::Tick(DeltaSeconds);
}

void ABoidDefaultPawn::SetupPlayerInputComponent(UInputComponent* InInputComponent) {
	Super::SetupPlayerInputComponent(InInputComponent);

	check(InInputComponent);

	InInputComponent->BindAction("AttachTarget", IE_Pressed, this, &ABoidDefaultPawn::ToggleAttachTarget);
	InInputComponent->BindAction("FollowRandom", IE_Pressed, this, &ABoidDefaultPawn::ToggleAttachToFollowRandom);
	InInputComponent->BindAction("Quit", IE_Pressed, this, &ABoidDefaultPawn::Quit);
	InInputComponent->BindAction("TargetDistanceUp", IE_Pressed, this, &ABoidDefaultPawn::ChangeCameraDistanceUp);
	InInputComponent->BindAction("TargetDistanceDown", IE_Pressed, this, &ABoidDefaultPawn::ChangeCameraDistanceDown);
}

void ABoidDefaultPawn::ToggleAttachTarget() {
	if (BoidTarget) {
		if (!IsAttachedToBoid)
			IsTargetAttached ? DetachTarget() : AttachTarget();
	} else {
		UE_LOG(BoidDefaultPawnLog, Warning, TEXT("Boid target not set"));
	}
}

void ABoidDefaultPawn::AttachTarget() {
	IsTargetAttached = true;
	BoidTarget->AttachToComponent(TargetAttachBoxComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
}

void ABoidDefaultPawn::DetachTarget() {
	IsTargetAttached = false;
	BoidTarget->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
}

void ABoidDefaultPawn::ToggleAttachToFollowRandom() {
	if (PlayerController) {
		if (!IsTargetAttached)
			IsAttachedToBoid ? DetachFromFollowRandom() : AttachToFollowRandom();
	} else {
		UE_LOG(BoidDefaultPawnLog, Warning, TEXT("Player controller not set"));
	}
}

void ABoidDefaultPawn::AttachToFollowRandom() {
	IsAttachedToBoid = true;

	UWorld* WorldContextObject = GetWorld();

	TArray<AActor*> AllBoids;
	UGameplayStatics::GetAllActorsOfClass(WorldContextObject, ABoid::StaticClass(), AllBoids);

	AActor* Boid = AllBoids[FMath::RandRange(0, AllBoids.Num() - 1)];
	PlayerController->SetViewTargetWithBlend(Boid, 3.f, EViewTargetBlendFunction::VTBlend_EaseInOut, 1.f);
}

void ABoidDefaultPawn::DetachFromFollowRandom() {
	IsAttachedToBoid = false;
	PlayerController->SetViewTargetWithBlend(this, 3.f, EViewTargetBlendFunction::VTBlend_EaseInOut, 1.f);
}

void ABoidDefaultPawn::ChangeCameraDistanceUp() {
	ChangeCameraDistance(1.f);
}

void ABoidDefaultPawn::ChangeCameraDistanceDown() {
	ChangeCameraDistance(-1.f);
}

void ABoidDefaultPawn::ChangeCameraDistance(float Direction) {
	TargetAttachStartingOffset.X = FMath::Clamp(TargetAttachStartingOffset.X + TargetDistanceChangePerTurn * Direction, TargetDistanceClampMin, TargetDistanceClampMax);
	TargetAttachBoxComponent->SetRelativeLocation(TargetAttachStartingOffset);
}

void ABoidDefaultPawn::Quit() {
	UKismetSystemLibrary::QuitGame(GetWorld(), PlayerController, EQuitPreference::Quit, false);
}
