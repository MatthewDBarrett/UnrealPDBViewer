#include "PDBImportLib.h"
#include <sstream>

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

TArray<FVector> UPDBImportLib::OutputAtomPositions(FString fileName) {

	TArray<FVector> atomPositions;

	FString directory = FPaths::ProjectContentDir();
	FString atomData;
	IPlatformFile& file = FPlatformFileManager::Get().GetPlatformFile();

	if (file.CreateDirectory(*directory)) {
		FString myFile = directory + "/" + fileName + ".pdb";
		FFileHelper::LoadFileToString(atomData, *myFile);

		FString Split1;
		atomData.Split("SCALE", NULL, &Split1, ESearchCase::IgnoreCase, ESearchDir::FromEnd);

		FString Split2;
		Split1.Split("0.00000", NULL, &Split2, ESearchCase::IgnoreCase, ESearchDir::FromEnd);

		FString Split3;
		Split2.Split("CONECT", &Split3, NULL, ESearchCase::IgnoreCase, ESearchDir::FromStart);

		TArray<FString> atomRecords;
		Split3.ParseIntoArray(atomRecords, TEXT(" "), true);

		int32 size = atomRecords.Num();

		for (int32 i = 0; i < atomRecords.Num(); i++) {
			if (atomRecords[i].Contains("ATOM") || atomRecords[i].Contains("ANISOU")) {
				FVector position;
				position = FVector(FCString::Atof(*atomRecords[i + 6]), FCString::Atof(*atomRecords[i + 7]), FCString::Atof(*atomRecords[i + 8]));

				atomPositions.Add(position);
			}
		}

	}

	return atomPositions;
}