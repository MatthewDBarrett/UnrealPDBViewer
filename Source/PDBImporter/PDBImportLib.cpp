// Fill out your copyright notice in the Description page of Project Settings.


#include "PDBImportLib.h"

FString UPDBImportLib::LoadFileToString(FString Filename) {

	FString directory = FPaths::ProjectContentDir();
	//FString directory = FPaths::GameSourceDir();
	FString result;
	IPlatformFile& file = FPlatformFileManager::Get().GetPlatformFile();

	if (file.CreateDirectory(*directory)) {
		//FString myFile = directory + "/PDB_Files" + "/" + Filename;
		FString myFile = directory + "/" + Filename;
		FFileHelper::LoadFileToString(result, *myFile);
	}

	return result;
}

FString UPDBImportLib::SaveStringToFile(FString Filename, FString Data) {
	FString directory = FPaths::ProjectContentDir();
	//FString directory = FPaths::GameSourceDir();
	FString result;
	IPlatformFile& file = FPlatformFileManager::Get().GetPlatformFile();

	if (file.CreateDirectory(*directory)) {
		//FString myFile = directory + "/PDB_Files" + "/" + Filename;
		FString myFile = directory + "/" + Filename;
		FFileHelper::SaveStringToFile(Data, *myFile);
	}

	return Filename;
}

TArray<FVector> UPDBImportLib::OutputAtomPositions(FString atomData) {

	FString Split1;
	atomData.Split("SCALE", NULL, &Split1, ESearchCase::IgnoreCase, ESearchDir::FromEnd);

	FString Split2;
	Split1.Split("0.00000", NULL, &Split2, ESearchCase::IgnoreCase, ESearchDir::FromEnd);

	FString Split3;
	Split2.Split("CONECT", &Split3, NULL, ESearchCase::IgnoreCase, ESearchDir::FromStart);
	

	TArray<FString> Out;
	Split3.ParseIntoArray(Out, TEXT(" "), true);

	TArray<FVector> atomPositions;

	int32 size = Out.Num();

	for (int32 i = 0; i < Out.Num(); i++) {
		if ( Out[i].Contains("ATOM") || Out[i].Contains("ANISOU")) {
			FVector position;
			position = FVector(FCString::Atof(*Out[i + 6]), FCString::Atof(*Out[i + 7]), FCString::Atof(*Out[i + 8]));

			atomPositions.Add(position);
		}
	}

	return atomPositions;
}