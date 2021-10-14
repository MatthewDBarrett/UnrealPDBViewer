#include "AtomData.h"

// Sets default values
AAtomData::AAtomData()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = mesh;
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

AAtomData::AAtomData(FString type, int32 serialNum, FString atomName, int32 branchIndicator, FString residueType, FString chainIdentifier,
	int32 residueSequenceNum, double xPos, double yPos, double zPos, FString elementSymbol) {

	this->type = type;
	this->serialNum = serialNum;
	this->atomName = atomName;
	this->branchIndicator = branchIndicator;
	this->residueType = residueType;
	this->chainIdentifier = chainIdentifier;
	this->residueSequenceNum = residueSequenceNum;
	this->xPos = xPos;
	this->yPos = yPos;
	this->zPos = zPos;
}

AAtomData::AAtomData(FString type, int32 serialNum, double xPos, double yPos, double zPos, FString elementSymbol) {
	this->type = type;
	this->serialNum = serialNum;
	this->xPos = xPos;
	this->yPos = yPos;
	this->zPos = zPos;
	this->elementSymbol = elementSymbol;
}

FVector AAtomData::GetPosition() { return FVector(xPos, yPos, zPos); }

FString AAtomData::GetType() { return type; }

int32 AAtomData::GetSerialNum() { return serialNum; }

FString AAtomData::GetAtomName() { return atomName; }

int32 AAtomData::GetBranchIndicator() { return branchIndicator; }

FString AAtomData::GetResidueType() { return residueType; }

FString AAtomData::GetChainIdentifier() { return chainIdentifier; }

int32 AAtomData::GetResidueSequenceNum() { return residueSequenceNum; }

double AAtomData::GetXPos() { return xPos; }

double AAtomData::GetYPos() { return yPos; }

double AAtomData::GetZPos() { return zPos; }

FString AAtomData::GetElementSymbol() { return elementSymbol; }


