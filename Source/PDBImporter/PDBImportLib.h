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

		//This Function allows the reading of a physical text file to a single string in-game at run time.
		UFUNCTION(BlueprintCallable, Category = "File I/O")
		static FString LoadFileToString(FString Filename);

		//This Function allows the Writing of a physical text file from a single string at run time.
		UFUNCTION(BlueprintCallable, Category = "File I/O")
		static FString SaveStringToFile(FString Filename, FString Data);

		//This Functin takes in atom records from PDB Files and outputs their co-ordinates in a 3D Array.
		UFUNCTION(BlueprintCallable, Category = "PDB")
		static TArray<FVector> OutputAtomPositions(FString atomData);

		//This Functin takes in atom records from PDB Files and outputs their co-ordinates in a 3D Array.
		UFUNCTION(BlueprintCallable, Category = "PDB")
		static TArray<FVector> OutputAtomPositions2(FString fileName);

};
