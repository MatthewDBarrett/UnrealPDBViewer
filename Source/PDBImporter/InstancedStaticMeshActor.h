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

	void SetPosition(FVector position);

	UPROPERTY(BlueprintReadWrite, Category = MyCharacter)
	int32 AtomsISM_index;

	void SetIndex(int32 index);

	UStaticMesh* StaticMesh;

	//UPROPERTY(VisibleAnywhere)
	//UPROPERTY(VisibleDefaultsOnly)
	UPROPERTY(BlueprintReadWrite, Category = MyCharacter)
	UInstancedStaticMeshComponent* InstancedStaticMeshComponent;

protected:

	virtual void BeginPlay() override;
};
