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

	UInstancedStaticMeshComponent *ISMComp;

	void InstanceAtom(FTransform instanceTransform);

	void RemoveInitialInstance();

	void SetCustomData(int32 index, int32 dataIndex, double dataValue, bool stateDirty);

	UStaticMesh* StaticMesh;

	//UPROPERTY(VisibleAnywhere)
	UPROPERTY(BlueprintReadWrite)
	UInstancedStaticMeshComponent* InstancedStaticMeshComponent;

protected:

	virtual void BeginPlay() override;
};
