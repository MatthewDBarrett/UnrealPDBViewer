// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include <Runtime\Core\Public\Misc\FileHelper.h>
#include "PDBImportLib.generated.h"

/**
 *
 */
UCLASS()
class PDBIMPORTER_API UPDBImportLib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

		//This Functin takes in atom records from PDB Files and outputs their co-ordinates in a 3D Array.
		UFUNCTION(BlueprintCallable, Category = "PDB")
		static TArray<FVector> OutputAtomPositions(FString fileName);

		//This Functin takes in atom records from PDB Files and outputs their co-ordinates in a 3D Array.
		UFUNCTION(BlueprintCallable, Category = "PDB")
		static TArray<FVector> OutputAtomPositions2(FString fileName);
};
