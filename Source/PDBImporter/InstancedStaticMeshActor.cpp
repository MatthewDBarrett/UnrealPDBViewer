#include "InstancedStaticMeshActor.h"

AInstancedStaticMeshActor::AInstancedStaticMeshActor() {
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	InstancedStaticMeshComponent = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("InstancedStaticMeshComponent"));
	SetRootComponent(InstancedStaticMeshComponent);
	InstancedStaticMeshComponent->SetMobility(EComponentMobility::Static);
	InstancedStaticMeshComponent->SetCollisionProfileName("Pawn");
}

void AInstancedStaticMeshActor::InstanceAtom(FTransform instanceTransform) {
	//InstancedStaticMeshComponent->SetCustomDataValue(0, 1.0f, 0, true);
	InstancedStaticMeshComponent->AddInstance( instanceTransform );
	
}

void AInstancedStaticMeshActor::RemoveInitialInstance() {
	InstancedStaticMeshComponent->RemoveInstance(0);
}

void AInstancedStaticMeshActor::BeginPlay() {
	Super::BeginPlay();
}




