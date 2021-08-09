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

	TArray<FString> Out;
	atomData.ParseIntoArray(Out, TEXT(" "), true);

	//FString output = "";
	//output = Out[6] + ", " + Out[7] + ", " + Out[8];

	TArray<FVector> atomPositions;

	//UE_LOG(LogClass, Log, *Out.Num());
	//UE_LOG(LogTemp, Error, TEXT("PhysicsHandle working %s"), *LineTraceEnding.ToString());
	int32 size = Out.Num();
	//UE_LOG(LogTemp, Warning, TEXT("Strings: %d"), size);

	for (int32 i = 0; i < Out.Num(); i++) {
		//if (Out[i].Equals("ATOM") || Out[i].Equals("ANISOU") ) {
		//UE_LOG(LogTemp, Warning, TEXT("%s"), *Out[i]);
		if ( Out[i].Contains("ATOM") || Out[i].Contains("ANISOU")) {

			//UE_LOG(LogTemp, Warning, TEXT("String %s"), *Out[i]);
			
			FVector position;
			position = FVector(FCString::Atof(*Out[i + 6]), FCString::Atof(*Out[i + 7]), FCString::Atof(*Out[i + 8]));

			UE_LOG(LogTemp, Warning, TEXT("VECTOR %s"), *position.ToString());
			
			//atomPositions[i] = FVector( FCString::Atof(*Out[6]) , FCString::Atof(*Out[7]), FCString::Atof(*Out[8]));
			atomPositions.Add(position);

			//UE_LOG(LogTemp, Warning, TEXT(Out[6] + ", " + Out[7] + ", " + Out[8]));
			//UE_LOG(LogTemp, Warning, TEXT("I just started running"));
		}
	}

	return atomPositions;
}