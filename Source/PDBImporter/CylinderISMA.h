#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CylinderISMA.generated.h"

UCLASS()
class PDBIMPORTER_API ACylinderISMA : public AActor
{
	GENERATED_BODY()
	
public:	
	ACylinderISMA();

	void InstanceConnection(FTransform instanceTransform);

	void SetPosition(FVector position);

	//UPROPERTY(VisibleAnywhere)
	UPROPERTY(VisibleDefaultsOnly)
	UInstancedStaticMeshComponent* InstancedStaticMeshComponent;

	UPROPERTY(BlueprintReadWrite, Category = MyCharacter)
	int32 ConnectionsISM_index;

	void SetIndex(int32 index);

protected:
	virtual void BeginPlay() override;
};
