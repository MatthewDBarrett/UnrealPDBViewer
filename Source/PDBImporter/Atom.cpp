#include "Atom.h"

Atom::Atom(FString type, int serialNum, FString atomName, int branchIndicator, FString residueType, FString chainIdentifier,
	int residueSequenceNum, double xPos, double yPos, double zPos) {

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

Atom::Atom(double xPos, double yPos, double zPos) {
	this->xPos = xPos;
	this->yPos = yPos;
	this->zPos = zPos;
}

Atom::~Atom() {

}

FVector Atom::GetPosition() { return FVector(xPos, yPos, zPos); }

FString Atom::GetType() { return type; }

int Atom::GetSerialNum() { return serialNum; }

FString Atom::GetAtomName() { return atomName; }

int Atom::GetBranchIndicator() { return branchIndicator; }

FString Atom::GetResidueType() { return residueType; }

FString Atom::GetChainIdentifier() { return chainIdentifier; }

int Atom::GetResidueSequenceNum() { return residueSequenceNum; }

double Atom::GetXPos() { return xPos; }

double Atom::GetYPos() { return yPos; }

double Atom::GetZPos() { return zPos; }
