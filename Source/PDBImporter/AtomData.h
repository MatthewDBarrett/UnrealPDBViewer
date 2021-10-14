#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AtomData.generated.h"

UCLASS()
class PDBIMPORTER_API AAtomData : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAtomData();

	AAtomData(FString type, int32 serialNum, FString atomName, int32 branchIndicator, FString residueType, FString chainIdentifier,
		int32 residueSequenceNum, double xPos, double yPos, double zPos, FString elementSymbol);
	AAtomData(FString type, int32 serialNum, double xPos, double yPos, double zPos, FString elementSymbol);

	FString GetType();
	int32 GetSerialNum();
	FString GetAtomName();
	int32 GetBranchIndicator();
	FString GetResidueType();
	FString GetChainIdentifier();
	int32 GetResidueSequenceNum();
	double GetXPos();
	double GetYPos();
	double GetZPos();
	FString GetElementSymbol();
	FVector GetPosition();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* mesh;

private:

	UPROPERTY(EditAnywhere)
		FString type;

	UPROPERTY(EditAnywhere)
		int32 serialNum;

	UPROPERTY(EditAnywhere)
		FString atomName;

	UPROPERTY(EditAnywhere)
		int32 branchIndicator;

	UPROPERTY(EditAnywhere)
		FString residueType;

	UPROPERTY(EditAnywhere)
		FString chainIdentifier;

	UPROPERTY(EditAnywhere)
		int32 residueSequenceNum;

	UPROPERTY(EditAnywhere)
		double xPos;

	UPROPERTY(EditAnywhere)
		double yPos;

	UPROPERTY(EditAnywhere)
		double zPos;

	UPROPERTY(EditAnywhere)
		FString elementSymbol;

};
