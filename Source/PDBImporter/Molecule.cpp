#include "Molecule.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/LineBatchComponent.h"
#include "Engine/World.h"

// Sets default values
AMolecule::AMolecule()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AMolecule::RenderConnection(FVector start, FVector end)
{
	GetWorld()->LineBatcher->DrawLine(start, end, FLinearColor::Red, 1, 1.0f);
	UE_LOG(LogTemp, Warning, TEXT("connection: %s <---> %s"), *start.ToString(), *end.ToString());
}

// Called when the game starts or when spawned
void AMolecule::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("started mol"));
}

// Called every frame
void AMolecule::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

