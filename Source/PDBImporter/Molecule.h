#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Atom.h"
#include "InstancedStaticMeshActor.h"
#include "CylinderISMA.h"
#include "Molecule.generated.h"

UCLASS()
class PDBIMPORTER_API AMolecule : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMolecule();

	UFUNCTION(BlueprintCallable)
	void ConvertPDB(FString fileName);

	bool isConnection(Atom a, Atom b);

	void SpawnAtoms();

	void SpawnSphere(FVector position, double size, FString atomName);

	void SpawnConnections();

	void SpawnCylinder(FVector atomPos1, FVector atomPos2);

	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
	TSubclassOf<AActor> ActorToSpawn;

	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
	TSubclassOf<AActor> StaticMeshToSpawn;

	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
	TSubclassOf<AActor> CylinderStaticMeshToSpawn;

	TArray<FString> atomNames;
	TArray<double> atomSizes;

	AInstancedStaticMeshActor* meshPointer;
	ACylinderISMA* cylinderMeshPointer;

	void SetAtomSizes();

	UPROPERTY()
	AActor* instancedStaticMeshActor;

	UPROPERTY()
	AActor* cylinderISMA;

	void SetAtomTypes();

	void SetAtomColours();

	void SpawnTempAtoms();

	void RemoveTempAtoms();

	double PositionsToRotation(FVector posA, FVector posB);

	TArray<AActor*> tempAtoms;

	TArray<Atom> atoms;

	int32 atomCount;

	FString moleculeName = "1ezg";

	int32 simulationScale = 50;

	double atomScale = 200;

	int32 searchBuffer = 10;

	double connectionThickness = 0.1f;

	bool renderConnections = true;
	
	bool renderHETAtoms = false;

	TArray<FString> atomTypes;

	TArray<FVector> atomColours;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
