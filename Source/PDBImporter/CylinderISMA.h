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

	//UPROPERTY(VisibleAnywhere)
	UPROPERTY(VisibleDefaultsOnly)
	UInstancedStaticMeshComponent* InstancedStaticMeshComponent;

protected:
	virtual void BeginPlay() override;
};
