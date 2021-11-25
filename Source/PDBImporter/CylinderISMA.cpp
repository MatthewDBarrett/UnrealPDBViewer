#include "CylinderISMA.h"

ACylinderISMA::ACylinderISMA()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	InstancedStaticMeshComponent = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("InstancedStaticMeshComponent"));
	SetRootComponent(InstancedStaticMeshComponent);
	InstancedStaticMeshComponent->SetMobility(EComponentMobility::Static);
	InstancedStaticMeshComponent->SetCollisionProfileName("Pawn");
}

void ACylinderISMA::InstanceConnection(FTransform instanceTransform) {
	InstancedStaticMeshComponent->AddInstance(instanceTransform);
}

void ACylinderISMA::SetPosition(FVector position) {
	SetActorLocation(position);
}

void ACylinderISMA::SetIndex(int32 index) { ConnectionsISM_index = index; }

void ACylinderISMA::BeginPlay()
{
	Super::BeginPlay();
	
}

