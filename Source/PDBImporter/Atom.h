#pragma once

#include "CoreMinimal.h"

class Atom {

public:
	Atom(FString type, int32 serialNum, FString atomName, int32 branchIndicator, FString residueType, FString chainIdentifier,
		int32 residueSequenceNum, double xPos, double yPos, double zPos);
	Atom(FString type, int32 serialNum, double xPos, double yPos, double zPos);
	~Atom();

	FString GetType();
	int32 GetSerialNum();
	FString GetAtomName();
	int32 GetBranchIndicator();
	FString GetResidueType();
	FString GetChainIdentifier();
	int32 GetResidueSequenceNum();
	double GetXPos();
	double GetYPos();
	double GetZPos();

	FVector GetPosition();
	
private:
	FString type;
	int32 serialNum;
	FString atomName;
	int32 branchIndicator;
	FString residueType;
	FString chainIdentifier;
	int32 residueSequenceNum;
	double xPos;
	double yPos;
	double zPos;

};
