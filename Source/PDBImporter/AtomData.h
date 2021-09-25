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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
