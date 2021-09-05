#pragma once

#include "CoreMinimal.h"

class Atom {

public:
	Atom(FString type, int serialNum, FString atomName, int branchIndicator, FString residueType, FString chainIdentifier,
		int residueSequenceNum, double xPos, double yPos, double zPos);
	Atom(double xPos, double yPos, double zPos);
	~Atom();

	FString GetType();
	int GetSerialNum();
	FString GetAtomName();
	int GetBranchIndicator();
	FString GetResidueType();
	FString GetChainIdentifier();
	int GetResidueSequenceNum();
	double GetXPos();
	double GetYPos();
	double GetZPos();

	FVector GetPosition();
	
private:
	FString type;
	int serialNum;
	FString atomName;
	int branchIndicator;
	FString residueType;
	FString chainIdentifier;
	int residueSequenceNum;
	double xPos;
	double yPos;
	double zPos;

};
