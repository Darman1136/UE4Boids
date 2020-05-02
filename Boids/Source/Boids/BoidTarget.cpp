#include "BoidTarget.h"
#include "Components/StaticMeshComponent.h"

ABoidTarget::ABoidTarget()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Mesh"));
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetRootComponent(Mesh);
}
