#include "Atom.h"

Atom::Atom(FString type, int32 serialNum, FString atomName, int32 branchIndicator, FString residueType, FString chainIdentifier,
	int32 residueSequenceNum, double xPos, double yPos, double zPos, FString elementSymbol) {

	this->type = type;
	this->serialNum = serialNum;
	this->atomName = atomName;
	this->branchIndicator = branchIndicator;
	this->residueType = residueType;
	this->chainIdentifier = chainIdentifier;
	this->residueSequenceNum = residueSequenceNum;
	this->xPos = xPos;
	this->yPos = yPos;
	this->zPos = zPos;
}

Atom::Atom(FString type, int32 serialNum, double xPos, double yPos, double zPos, FString elementSymbol) {
	this->type = type;
	this->serialNum = serialNum;
	this->xPos = xPos;
	this->yPos = yPos;
	this->zPos = zPos;
	this->elementSymbol = elementSymbol;
}

Atom::~Atom() {

}

FVector Atom::GetPosition() { return FVector(xPos, yPos, zPos); }

void Atom::SetRadius(double radius) { atomRadius = radius; }

double Atom::GetRadius() { return atomRadius; }

FString Atom::GetType() { return type; }

int32 Atom::GetSerialNum() { return serialNum; }

FString Atom::GetAtomName() { return atomName; }

int32 Atom::GetBranchIndicator() { return branchIndicator; }

FString Atom::GetResidueType() { return residueType; }

FString Atom::GetChainIdentifier() { return chainIdentifier; }

int32 Atom::GetResidueSequenceNum() { return residueSequenceNum; }

double Atom::GetXPos() { return xPos; }

double Atom::GetYPos() { return yPos; }

double Atom::GetZPos() { return zPos; }

FString Atom::GetElementSymbol() { return elementSymbol; }
