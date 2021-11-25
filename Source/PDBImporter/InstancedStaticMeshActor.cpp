#include "InstancedStaticMeshActor.h"
#include "GameFramework/Actor.h"

AInstancedStaticMeshActor::AInstancedStaticMeshActor() {
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	InstancedStaticMeshComponent = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("InstancedStaticMeshComponent"));
	SetRootComponent(InstancedStaticMeshComponent);
	InstancedStaticMeshComponent->SetMobility(EComponentMobility::Static);
	InstancedStaticMeshComponent->SetCollisionProfileName("Pawn");
}

void AInstancedStaticMeshActor::InstanceAtom(FTransform instanceTransform) {
	InstancedStaticMeshComponent->AddInstance( instanceTransform );
}

void AInstancedStaticMeshActor::RemoveInitialInstance() {
	InstancedStaticMeshComponent->RemoveInstance(0);
}

void AInstancedStaticMeshActor::SetCustomData(int32 index, int32 dataIndex, double dataValue, bool stateDirty) {
	InstancedStaticMeshComponent->SetCustomDataValue(index, dataIndex, dataValue, stateDirty);
}

void AInstancedStaticMeshActor::SetPosition(FVector position) { SetActorLocation(position); }

void AInstancedStaticMeshActor::SetIndex(int32 index) {	AtomsISM_index = index; }

void AInstancedStaticMeshActor::BeginPlay() {
	Super::BeginPlay();
}




