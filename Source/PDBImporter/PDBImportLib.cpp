// Fill out your copyright notice in the Description page of Project Settings.


#include "PDBImportLib.h"
#include <Runtime\Core\Public\Misc\Paths.h>
#include <Runtime\Core\Public\HAL\PlatformFilemanager.h>

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

TArray<FVector> UPDBImportLib::OutputAtomPositions2(FString fileName) {

	TArray<FVector> atomPositions;

	FString directory = FPaths::ProjectContentDir();
	IPlatformFile& file = FPlatformFileManager::Get().GetPlatformFile();

	//FString result1;

	if (file.CreateDirectory(*directory)) {
		FString myFile = directory + "/" + fileName + ".pdb";
		//UE_LOG(LogTemp, Warning, TEXT("FileDir: %s"), *myFile);
		const char* fileLocation = TCHAR_TO_ANSI(*myFile);

		//UE_LOG(LogTemp, Warning, TEXT("STRING: %c"), *myFile);

		FILE* ptr = fopen(fileLocation, "r");

		//FFileHelper::LoadFileToString(result1, *myFile);

		//UE_LOG(LogTemp, Warning, TEXT("STRING: %s"), *result1);

		if (ptr == NULL) {
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("FILE NOT FOUND."));
			return atomPositions;
		}
		else {
			double xValue = 0;
			double yValue = 0;
			double zValue = 0;
			//char firstWord[255];

			//UE_LOG(LogTemp, Warning, TEXT("RUNNING"));

			//while (fscanf(ptr, "%*s %*d %*s %*s %*s %*d %lf %lf %lf %*lf %*lf %*s\n", &xValue, &yValue, &zValue) == 3 ) {
			//	UE_LOG(LogTemp, Warning, TEXT("X: %lf, Y: %lf, Z: %lf"), xValue, yValue, zValue);
			//}

			int count = 0;
			
			char buffer[200];
			while (fgets(buffer, sizeof buffer, ptr)) {
				if (sscanf(buffer, "%*s %*d %*s %*s %*s %*d %lf %lf %lf %*lf %*lf %*s\n",
					&xValue, &yValue, &zValue) == 3) {
					//fprintf(stderr, "Unable to parse <%s>\n", buffer);
					//exit(-1);
					//UE_LOG(LogTemp, Warning, TEXT("X: %lf, Y: %lf, Z: %lf"), xValue, yValue, zValue);

					FVector position;
					position = FVector(xValue, yValue, zValue);
					
					atomPositions.Add(position);

					count++;
				}
				//UE_LOG(LogTemp, Warning, TEXT("X: %lf, Y: %lf, Z: %lf"), xValue, yValue, zValue);
			}

			UE_LOG(LogTemp, Warning, TEXT("Atoms: %d"), count);

			//while (fscanf(ptr, "%*s %*5d%*1c%*4c%*1c%*3c%*c%*1c%*4d%*1c%*1c%*1c%*1c%8lf%8lf%8lf%*6f%*6f%*1c%*1c%*1c%*1c%*1c%*1c%*4c%*2c%*2c", &xValue, &yValue, &zValue) != EOF) {
			//	UE_LOG(LogTemp, Warning, TEXT("X: %lf, Y: %lf, Z: %lf"), xValue, yValue, zValue);
			//}

			//char temp[3000];

			//while (fscanf(ptr, "%s %*d %*s %*s %*s %*d %*lf %*lf %*lf %*lf %*lf %*s\n", temp) != EOF) {
			//	UE_LOG(LogTemp, Warning, TEXT("First String: %c"), *temp);
			//}

			//const char atom_line_iformat_[] =
			//"ATOM  %5d%*1c%4c%1c%3c%*c%1c%4d%1c%*1c%*1c%*1c%8f%8f%8f%6f%6f%*1c%*1c%*1c%*1c%*1c%*1c%4c%2c%2c";

			//int snum = -1;
			//char name[5] = { '\0','\0','\0','\0','\0', };
			//char alt = '\0';
			//char resname[4] = { '\0','\0','\0','\0' };
			//char chain;
			//int resnum = -1;
			//char insertion_residue_code;
			//float x, y, z;
			//float occupancy, tempFactor;
			//char segID[5] = { '\0','\0','\0','\0','\0' };
			//char element[3] = { '\0','\0','\0' };
			//char charge[3] = { '\0','\0','\0' };

			//while (fscanf(ptr, atom_line_iformat_, &snum, name, &alt, resname, &chain, &resnum, &insertion_residue_code,
			//	&x, &y, &z, &occupancy, &tempFactor, segID, element, charge) != EOF) {
			//	//UE_LOG(LogTemp, Warning, TEXT("X: %f, Y: %f, Z: %f"), x, y, z);
			//}
		}

		//int32 size = Out.Num();

		//for (int32 i = 0; i < Out.Num(); i++) {
		//	if (Out[i].Contains("ATOM") || Out[i].Contains("ANISOU")) {
		//		FVector position;
		//		position = FVector(FCString::Atof(*Out[i + 6]), FCString::Atof(*Out[i + 7]), FCString::Atof(*Out[i + 8]));
		//
		//		atomPositions.Add(position);
		//	}
		//}
		return atomPositions;
	}
	return atomPositions;
}