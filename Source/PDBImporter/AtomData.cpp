#include "AtomData.h"

// Sets default values
AAtomData::AAtomData()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAtomData::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAtomData::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

