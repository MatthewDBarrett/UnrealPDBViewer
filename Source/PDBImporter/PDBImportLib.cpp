#include "PDBImportLib.h"
#include <sstream>
#include <cstdlib>
#include <algorithm>
#include <cctype>
#include <string>
#include <iostream>
#include "Atom.h"

bool is_number(const std::string& s)
{
	char* end = nullptr;
	double val = strtod(s.c_str(), &end);
	return end != s.c_str() && *end == '\0' && val != HUGE_VAL;
}

bool has_any_digits(const std::string& s)
{
	return std::any_of(s.begin(), s.end(), ::isdigit);
}

TArray<FVector> GetAtomPositions(TArray<Atom> atoms) {
	TArray<FVector> atomPositions;

	for (int i = 0; i < atoms.Num(); i++) {
		atomPositions.Add(atoms[i].GetPosition());
	}

	return atomPositions;
}

TArray<FVector> UPDBImportLib::OutputAtomPositions(FString fileName) {

	TArray<Atom> atoms;

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
			if (atomRecords[i].Contains("ATOM") || atomRecords[i].Contains("ANISOU") || atomRecords[i].Contains("HETATM")) {

				for (int j = 1; j < 7; j++) {

					FString sample = atomRecords[i + j];
					std::string test2 = std::string(TCHAR_TO_UTF8(*sample));

					if (!is_number(test2)) {
						FString sample2 = atomRecords[i + j + 1];
						std::string test3 = std::string(TCHAR_TO_UTF8(*sample2));

						if (has_any_digits(test3)) {
							atoms.Add( Atom(atomRecords[i], FCString::Atoi(*atomRecords[i + 1]), FCString::Atof(*atomRecords[i + j + 2]), FCString::Atof(*atomRecords[i + j + 3]), FCString::Atof(*atomRecords[i + j + 4])) );
							break;
						}
					}
				}	
			}
		}

		UE_LOG(LogTemp, Warning, TEXT("total atoms: %d"), atoms.Num());

		for (int k = 0; k < atoms.Num(); k++) {
			int temp = atoms[k].GetSerialNum();
			UE_LOG(LogTemp, Warning, TEXT("serial num: %d"), temp);
		}

	}
	return GetAtomPositions( atoms );
}