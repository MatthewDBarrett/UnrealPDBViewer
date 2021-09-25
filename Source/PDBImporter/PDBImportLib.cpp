#include "PDBImportLib.h"
#include <sstream>
#include <cstdlib>
#include <algorithm>
#include <cctype>
#include <string>
#include <iostream>
#include "Atom.h"
#include "Molecule.h"

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

double GetAtomSize(FString elementName) {
	FString directory = FPaths::ProjectContentDir();
	FString atomData;
	IPlatformFile& file = FPlatformFileManager::Get().GetPlatformFile();

	if (file.CreateDirectory(*directory)) {
		FString myFile = directory + "/atomData.art";
		FFileHelper::LoadFileToString(atomData, *myFile);

		FString Split1;
		atomData.Split("START", NULL, &Split1, ESearchCase::IgnoreCase, ESearchDir::FromStart);

		FString Split2;
		Split1.Split("END", &Split2, NULL, ESearchCase::IgnoreCase, ESearchDir::FromEnd);

		TArray<FString> stringRecords;
		Split2.ParseIntoArray(stringRecords, TEXT(" "), true);

		for (int32 i = 0; i < stringRecords.Num(); i++) {
			if (stringRecords[i].Contains( elementName )) {
				return FCString::Atod(*stringRecords[i + 2]);
			}
		}
	}

	return 0;
}

bool isConnection(Atom a, Atom b) {
	FVector aPos = a.GetPosition();
	FVector bPos = b.GetPosition();

	double aRadius = ( GetAtomSize(a.GetElementSymbol()) ) / 2;
	double bRadius = ( GetAtomSize(b.GetElementSymbol()) ) / 2;

	double distance = (aPos - bPos).Size();

	if (distance - aRadius - bRadius <= 0)
		return true;
	else
		return false;
}

void computeConnections(TArray<Atom> atoms) {
	AMolecule mol;
	for (int i = 0; i < atoms.Num(); i++) {
		for (int j = 0; j < atoms.Num(); j++) {
			if (isConnection(atoms[i], atoms[j])) {
				//mol.RenderConnection(atoms[i].GetPosition(), atoms[j].GetPosition());
			}
		}
	}
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

						FString atomName = atomRecords[i + 2];							//Atom Type
						atomName = atomName.LeftChop( atomRecords[i + 2].Len() - 1 );

						if (has_any_digits(test3)) {
							//atoms.Add( Atom(atomRecords[i], FCString::Atoi(*atomRecords[i + 1]), atomName, FCString::Atof(*atomRecords[i + j + 2]), FCString::Atof(*atomRecords[i + j + 3]), FCString::Atof(*atomRecords[i + j + 4])) );
							break;
						}
					}
				}	
			}
		}

		UE_LOG(LogTemp, Warning, TEXT("total atoms records: %d"), atoms.Num());

		for (int k = 0; k < atoms.Num(); k++) {
			FString temp = atoms[k].GetAtomName();
			UE_LOG(LogTemp, Warning, TEXT("atom Name: %s"), *temp);
		}

	}
	return GetAtomPositions( atoms );
}

TArray<FVector> UPDBImportLib::OutputAtomPositions2(FString fileName) {
	TArray<Atom> atoms;

	FString directory = FPaths::ProjectContentDir();
	FString atomData;
	IPlatformFile& file = FPlatformFileManager::Get().GetPlatformFile();

	if (file.CreateDirectory(*directory)) {
		FString myFile = directory + "/" + fileName + ".pdb";
		FFileHelper::LoadFileToString(atomData, *myFile);

		TArray<FString> stringRecords;
		atomData.ParseIntoArray(stringRecords, TEXT(" "), true);

		for (int32 i = 0; i < stringRecords.Num(); i++) {
			if (stringRecords[i].Contains("ATOM") || stringRecords[i].Contains("ANI@SOU") || stringRecords[i].Contains("HET@ATM")) {
				if (stringRecords[i + 1].IsNumeric()) {
					for (int j = 1; j < 7; j++) {

						FString sample = stringRecords[i + j];
						std::string sampleString = std::string(TCHAR_TO_UTF8(*sample));

						if (!is_number(sampleString)) {
							FString sample2 = stringRecords[i + j + 1];
							std::string sampleString2 = std::string(TCHAR_TO_UTF8(*sample2));

							if (has_any_digits(sampleString2)) {
								FString elementName;

								for (int o = 0; o < 10; o++) {
									if (stringRecords[i + j + o].Contains("ATOM") || stringRecords[i + j + o].Contains("ANISOU") ||
										stringRecords[i + j + o].Contains("HETATM") || stringRecords[i + j + o].Contains("TER")) {
										elementName = stringRecords[i + j + o - 1];
										break;
									}
								}

								atoms.Add(Atom(stringRecords[i], FCString::Atoi(*stringRecords[i + 1]), FCString::Atof(*stringRecords[i + j + 2]),
									FCString::Atof(*stringRecords[i + j + 3]), FCString::Atof(*stringRecords[i + j + 4]), elementName));
								break;
							}
						}
					}
				}
			}

		}
		UE_LOG(LogTemp, Warning, TEXT("total atoms records: %d"), atoms.Num());

		for (int k = 0; k < atoms.Num(); k++) {
			FString temp = atoms[k].GetElementSymbol();
			FString size = FString::SanitizeFloat(GetAtomSize(atoms[k].GetElementSymbol()));
			UE_LOG(LogTemp, Warning, TEXT("atom Name: %s size: %s"), *temp, *size);
		}
	}
	return GetAtomPositions(atoms);
}

TArray<FVector> UPDBImportLib::OutputStartConnections(FString fileName)
{
	return TArray<FVector>();
}

TArray<FVector> UPDBImportLib::OutputEndConnections(FString fileName)
{
	return TArray<FVector>();
}
