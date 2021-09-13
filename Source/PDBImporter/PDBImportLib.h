// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Atom.h"
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
		static TArray<FVector> OutputAtoms(FString fileName);

		//This Functin takes in atom records from PDB Files and outputs their co-ordinates in a 3D Array.
		UFUNCTION(BlueprintCallable, Category = "PDB")
		static TArray<Atom> OutputAtoms2(FString fileName);

		//This Functin takes in atom records from PDB Files and outputs their co-ordinates in a 3D Array.
		UFUNCTION(BlueprintCallable, Category = "PDB")
		static TArray<FVector> OutputAtomCon1(TArray<Atom> atoms);

		//This Functin takes in atom records from PDB Files and outputs their co-ordinates in a 3D Array.
		UFUNCTION(BlueprintCallable, Category = "PDB")
		static TArray<FVector> OutputAtomCon2(TArray<Atom> atoms);

		//This Functin takes in atom records from PDB Files and outputs their co-ordinates in a 3D Array.
		UFUNCTION(BlueprintCallable, Category = "PDB")
		static TArray<FVector> OutputAtomPositions(TArray<Atom> atoms);
};
