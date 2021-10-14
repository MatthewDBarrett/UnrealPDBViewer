#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InstancedStaticMeshActor.generated.h"

UCLASS()
class PDBIMPORTER_API AInstancedStaticMeshActor : public AActor
{
	GENERATED_BODY()
	
public:	

	AInstancedStaticMeshActor();

	void InstanceAtom(FTransform instanceTransform);

	UPROPERTY(VisibleAnywhere)
	UInstancedStaticMeshComponent* InstancedStaticMeshComponent;

protected:

	virtual void BeginPlay() override;
};
