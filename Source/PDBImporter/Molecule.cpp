#include "Molecule.h"
#include "Components/LineBatchComponent.h"
#include "Engine/World.h"
#include "Components/StaticMeshComponent.h"
#include "Atom.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "InstancedStaticMeshActor.h"
#include "CylinderISMA.h"

// Sets default values
AMolecule::AMolecule()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMolecule::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMolecule::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (MoleculeCreated == false) {
		if (moleculeName != "") {
			this->CreateMolecule();
			MoleculeCreated = true;
		}
	}
}

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

void AMolecule::CreateMolecule() {
	FActorSpawnParameters SpawnParams;
	FVector pos = FVector(0, 0, 0);
	FRotator rot = FRotator(0, 0, 0);

	instancedStaticMeshActor = GetWorld()->SpawnActor<AActor>(StaticMeshToSpawn, pos, rot, SpawnParams);
	cylinderISMA = GetWorld()->SpawnActor<AActor>(CylinderStaticMeshToSpawn, pos, rot, SpawnParams);

	meshPointer = Cast<AInstancedStaticMeshActor>(instancedStaticMeshActor);
	cylinderMeshPointer = Cast<ACylinderISMA>(cylinderISMA);

	if (molIndex != NULL) {
		meshPointer->SetIndex(molIndex);
		cylinderMeshPointer->SetIndex(molIndex);
	}

	this->SetAtomTypes();
	this->SetAtomColours();

	this->ConvertPDB(moleculeName);
	this->SetAtomSizes();
	this->SetProteinCentre();

	if (renderConnections) {
		this->SpawnTempAtoms();
		this->SpawnConnections();
		this->RemoveTempAtoms();
		//this->CreateNonStandardConnections();
	}

	this->SpawnAtoms();
}

void AMolecule::CreateNonStandardConnections() {
	FString directory = FPaths::ProjectContentDir();
	FString atomData;
	IPlatformFile& file = FPlatformFileManager::Get().GetPlatformFile();

	if (file.CreateDirectory(*directory)) {
		FString myFile = directory + "/" + moleculeName + ".pdb";
		FFileHelper::LoadFileToString(atomData, *myFile);

		TArray<FString> stringRecords;
		atomData.ParseIntoArray(stringRecords, TEXT(" "), true);

		for (int32 i = 0; i < stringRecords.Num(); i++) {
			if (stringRecords[i].Contains("CONECT")) {
				for (int32 j = 1; j < 20; j++) {
					if (stringRecords[i + j + 1].Contains("MASTER") || stringRecords[i + j + 1].Contains("CONECT"))
						break;

						if (FCString::Atoi(*stringRecords[i + j]) <= atoms.Num() && FCString::Atoi(*stringRecords[i + j + 1]) <= atoms.Num()) {			//if the atom has been displayed (incase HET atoms not being shown)
							//UE_LOG(LogTemp, Log, TEXT("StringA: %s"), *stringRecords[i + j]);
							//UE_LOG(LogTemp, Log, TEXT("StringB: %s"), *stringRecords[i + j + 1]);
							FVector atomA = atoms[FCString::Atoi(*stringRecords[i + j]) - 1].GetPosition();
							FVector atomB = atoms[FCString::Atoi(*stringRecords[i + j + 1]) - 1].GetPosition();
							this->SpawnCylinder(atomA * simulationScale, atomB * simulationScale);
						}
				}
			}
		}
	}
}

void AMolecule::SetAtomSizes() {
	FString directory = FPaths::ProjectContentDir();
	FString atomData;
	IPlatformFile& file = FPlatformFileManager::Get().GetPlatformFile();

	if (file.CreateDirectory(*directory)) {
		FString myFile = directory + "/" + "/PDB_Files/" + "/atomData.art";
		FFileHelper::LoadFileToString(atomData, *myFile);

		FString Split1;
		atomData.Split("START", NULL, &Split1, ESearchCase::IgnoreCase, ESearchDir::FromStart);

		FString Split2;
		Split1.Split("END", &Split2, NULL, ESearchCase::IgnoreCase, ESearchDir::FromEnd);

		TArray<FString> stringRecords;
		Split2.ParseIntoArray(stringRecords, TEXT(" "), true);

		for (int i = 0; i < atoms.Num(); i++) {
			int32 index = (stringRecords.Find(atoms[i].GetElementSymbol()));
			FString radiusString = stringRecords[index + 2];
			double radiusDouble = FCString::Atod(*radiusString);
			atoms[i].SetRadius((FCString::Atod(*radiusString)) / 0.775f);
			//UE_LOG(LogTemp, Warning, TEXT("radius: %lf"), radiusDouble);
		}
	}
}

void AMolecule::SetFileName(FString fileName) {
	moleculeName = fileName;
}

void AMolecule::SetAtomTypes() {
	atomTypes.Add(TEXT("H"));
	atomTypes.Add(TEXT("C"));
	atomTypes.Add(TEXT("N"));
	atomTypes.Add(TEXT("O"));
	atomTypes.Add(TEXT("F"));
	atomTypes.Add(TEXT("Cl"));
	atomTypes.Add(TEXT("Br"));
	atomTypes.Add(TEXT("I"));
	atomTypes.Add(TEXT("He"));
	atomTypes.Add(TEXT("Ne"));
	atomTypes.Add(TEXT("Ar"));
	atomTypes.Add(TEXT("Kr"));
	atomTypes.Add(TEXT("Xe"));
	atomTypes.Add(TEXT("Rn"));
	atomTypes.Add(TEXT("Og"));
	atomTypes.Add(TEXT("P"));
	atomTypes.Add(TEXT("S"));
	atomTypes.Add(TEXT("B"));
	atomTypes.Add(TEXT("Li"));
	atomTypes.Add(TEXT("Na"));
	atomTypes.Add(TEXT("K"));
	atomTypes.Add(TEXT("Rb"));
	atomTypes.Add(TEXT("Cs"));
	atomTypes.Add(TEXT("Fr"));
	atomTypes.Add(TEXT("Be"));
	atomTypes.Add(TEXT("Mg"));
	atomTypes.Add(TEXT("Ca"));
	atomTypes.Add(TEXT("Sr"));
	atomTypes.Add(TEXT("Ba"));
	atomTypes.Add(TEXT("Ra"));
	atomTypes.Add(TEXT("Ti"));
	atomTypes.Add(TEXT("Fe"));
}

void AMolecule::SetAtomColours() {
	atomColours.Add(FVector(255, 255, 255));		//H
	atomColours.Add(FVector(50,  50,  50));			//C  CPK is 144,144,144
	atomColours.Add(FVector(48,  80,  248));		//N
	atomColours.Add(FVector(255, 13,  13));			//O
	atomColours.Add(FVector(144, 224, 80));			//F
	atomColours.Add(FVector(31,  240, 31));			//Cl
	atomColours.Add(FVector(166, 41,  41));			//Br
	atomColours.Add(FVector(148, 0,   148));		//I
	atomColours.Add(FVector(217, 255, 255));		//He
	atomColours.Add(FVector(179, 227, 245));		//Ne
	atomColours.Add(FVector(128, 209, 227));		//Ar
	atomColours.Add(FVector(92,  184, 209));		//Kr
	atomColours.Add(FVector(66,  158, 176));		//Xe
	atomColours.Add(FVector(66,  130, 150));		//Rn
	atomColours.Add(FVector(255, 132, 212));		//Og  Default Pink
	atomColours.Add(FVector(255, 128, 0));			//P
	atomColours.Add(FVector(255, 255, 48));			//S
	atomColours.Add(FVector(255, 181, 181));		//B
	atomColours.Add(FVector(204, 128, 255));		//Li
	atomColours.Add(FVector(171, 92,  242));		//Na
	atomColours.Add(FVector(143, 64,  212));		//K
	atomColours.Add(FVector(112, 46,  176));		//Rb
	atomColours.Add(FVector(87,  23,  143));		//Cs
	atomColours.Add(FVector(66,  0,   102));		//Fr
	atomColours.Add(FVector(194, 255, 0));			//Be
	atomColours.Add(FVector(138, 255, 0));			//Mg
	atomColours.Add(FVector(61,  255, 0));			//Ca
	atomColours.Add(FVector(0,   255, 0));			//Sr
	atomColours.Add(FVector(0,   201, 0));			//Ba
	atomColours.Add(FVector(0,   125, 0));			//Ra
	atomColours.Add(FVector(191, 194, 199));		//Ti
	atomColours.Add(FVector(224, 102, 51));			//Fe
}

void AMolecule::SpawnTempAtoms() {
	for (Atom atom : atoms) {
		FVector position = FVector(atom.GetXPos() * simulationScale, atom.GetYPos() * simulationScale, atom.GetZPos() * simulationScale);
		double size = ((atom.GetRadius() * atomScale) * simulationScale);

		this->SpawnSphere(position, size, atom.GetElementSymbol());
	}
}

void AMolecule::RemoveTempAtoms() {
	for (AActor* actor : tempAtoms) {
		if (actor != nullptr)
			actor->Destroy();
	}
}

void AMolecule::SetAtomSize(float size) {
	atomScale = size/100;

	if (MoleculeCreated == true) {
		this->SpawnAtoms();
	}
}

void AMolecule::SetPosition(FVector position) {
	if (meshPointer != nullptr) {
		meshPointer->SetPosition(position);
		cylinderMeshPointer->SetPosition(position);
	}
}

void AMolecule::SetMoleculeIndex(int32 index) {
	molIndex = index;
}

FString AMolecule::GetFileName() {
	if (moleculeName.IsEmpty())
		return "";
	else
		return moleculeName;
}

int32 AMolecule::GetNumAtoms() {
	return atomCount;
}

int32 AMolecule::GetNumConnections() {
	return connectionCount;
}

bool AMolecule::isConnection(Atom a, Atom b) {
	double aRadius = a.GetRadius()/2;
	double bRadius = b.GetRadius()/2;

	double distance = FVector::Distance(a.GetPosition(), b.GetPosition());

	if (distance < 0) { distance = -distance; }			//absolute value

	//UE_LOG(LogTemp, Log, TEXT("distance: %lf - radiusA: %lf - radiusB: %lf"), distance, aRadius, bRadius);

	if (distance - aRadius - bRadius <= 0) {
		//UE_LOG(LogTemp, Log, TEXT("TRUE . AtomA: %d AtomB: %d . distance: %lf - radiusA: %lf - radiusB: %lf"), a.GetSerialNum(), b.GetSerialNum(), distance, aRadius, bRadius);
		return true;
	}
	else {
		//UE_LOG(LogTemp, Log, TEXT("FALSE . AtomA: %d AtomB: %d . distance: %lf - radiusA: %lf - radiusB: %lf"), a.GetSerialNum(), b.GetSerialNum(), distance, aRadius, bRadius);
		return false;
	}
}

void AMolecule::ConvertPDB(FString fileName) {
	FString directory = FPaths::ProjectContentDir();
	FString atomData;
	IPlatformFile& file = FPlatformFileManager::Get().GetPlatformFile();

	if (file.CreateDirectory(*directory)) {
		FString myFile = directory + "/" + "/PDB_Files/" + fileName + ".pdb";
		//UE_LOG(LogTemp, Warning, TEXT("Directory: %s"), *myFile);
		FFileHelper::LoadFileToString(atomData, *myFile);

		TArray<FString> stringRecords;
		atomData.ParseIntoArray(stringRecords, TEXT(" "), true);

		//GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Red, FString::FromInt(stringRecords.Num()));

		for (int32 i = 0; i < stringRecords.Num(); i++) {
			FString hetString = TEXT("HET@ATM");
			if (renderHETAtoms)
				hetString = TEXT("HETATM");
			if (stringRecords[i].Contains("ATOM") || stringRecords[i].Contains("ANI@SOU") || stringRecords[i].Contains(hetString)) {
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

								double xPos = FCString::Atof(*stringRecords[i + j + 2]);
								double yPos = FCString::Atof(*stringRecords[i + j + 3]);
								double zPos = FCString::Atof(*stringRecords[i + j + 4]);

								Atom tempAtom = Atom(stringRecords[i], FCString::Atoi(*stringRecords[i + 1]), xPos, yPos, zPos, elementName);

								atoms.Add(tempAtom);
								break;
							}
						}
					}
				}
			}

		}
		//UE_LOG(LogTemp, Warning, TEXT("total atoms records: %d"), atoms.Num());

		//for (int k = 0; k < atoms.Num(); k++) {
		//	FString temp = atoms[k]->GetElementSymbol();
		//	FString size = FString::SanitizeFloat(GetAtomSize(atoms[k]->GetElementSymbol()));
		//	UE_LOG(LogTemp, Warning, TEXT("atom Name: %s size: %s"), *temp, *size);
		//}
	}
	//return GetAtomPositions(atoms);
	//return TArray<FVector>();
}

void AMolecule::SpawnAtoms() {
	int32 count = 1;
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::FromInt(atoms.Num()));
	for (Atom atom : atoms) {
		//UE_LOG(LogTemp, Warning, TEXT("atom Name: %s"), *atom.GetElementSymbol());

		FVector position = FVector(atom.GetXPos() - proteinCentre.X, atom.GetYPos() - proteinCentre.Y, atom.GetZPos() - proteinCentre.Z);
		position = FVector(position.X * simulationScale, position.Y * simulationScale, position.Z * simulationScale);
		double size = (atom.GetRadius() * atomScale) * simulationScale;

		//this->SpawnSphere(position, size, atom.GetElementSymbol());
		
		FRotator rot = FRotator(0, 0, 0);
		FVector scale = FVector(size, size, size);
		
		FTransform transform = FTransform(rot, position, scale);
		
		if (meshPointer != nullptr) {
			meshPointer->InstanceAtom(transform);

			int32 atomIndex = atomTypes.Find(atom.GetElementSymbol());

			if ( (atomIndex - 1) < atomColours.Num() ) {
				FVector atomColour = atomColours[atomIndex];

				meshPointer->SetCustomData(count, 0, atomColour.X / 255, true);

				meshPointer->SetCustomData(count, 1, atomColour.Y / 255, true);

				meshPointer->SetCustomData(count, 2, atomColour.Z / 255, true);
			}
		}
		//UE_LOG(LogTemp, Log, TEXT("name: %s"), *test);	
		count++;
	}
	meshPointer->RemoveInitialInstance();

}

void AMolecule::SpawnSphere(FVector position, double size, FString atomName) {
	FActorSpawnParameters SpawnParams;
	FRotator rotation = FRotator(0.0f, 0.0f, 0.0f);
	atomCount++;

	AActor* SpawnedActorRef = GetWorld()->SpawnActor<AActor>(ActorToSpawn, position, rotation, SpawnParams);
	SpawnedActorRef->SetActorScale3D(FVector((size )));

	atomName.Append( "-Atom_" );							//String Formatting
	FString CountString = FString::FromInt(atomCount);
	atomName.Append(CountString);

	SpawnedActorRef->Tags.Add(FName(atomName));				//Setting Label

	tempAtoms.Add(SpawnedActorRef);
}

void AMolecule::SpawnConnections() {

	int32 connectionNum = 0;

	TArray<TEnumAsByte<EObjectTypeQuery>> traceObjectTypes;
	traceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
	TArray<AActor*> ignoreActors;
	TArray<AActor*> outActors;
	float searchRadius;
	FVector atomPosition;
	UClass* seekClass = AActor::StaticClass();


	for (Atom atom : atoms) {
		searchRadius = ( atom.GetRadius() * 2 ) * simulationScale;
		atomPosition = atom.GetPosition() * simulationScale;

		UKismetSystemLibrary::SphereOverlapActors(GetWorld(), atomPosition, searchRadius, traceObjectTypes, seekClass, ignoreActors, outActors);

		for (AActor* overlappedActor : outActors) {
			if (overlappedActor->Tags.Num() != 0) {
				FString overlappedName = overlappedActor->Tags[0].ToString();

				if (overlappedName.Contains("Atom")) {

					FString Left, Right;
					overlappedName.Split(TEXT("_"), &Left, &Right);

					int32 overlappedIndex = (FCString::Atoi(*Right) - 1);

					if (overlappedIndex < atom.GetSerialNum()) {
						if (atom.GetSerialNum() != atoms[overlappedIndex].GetSerialNum()) {				//Check that the same atom is not comparing to itself
							if (this->isConnection(atom, atoms[overlappedIndex])) {
								connectionNum++;

								FVector atomPos1 = FVector(atom.GetPosition().X - proteinCentre.X, atom.GetPosition().Y - proteinCentre.Y, atom.GetPosition().Z - proteinCentre.Z);
								FVector atomPos2 = FVector(atoms[overlappedIndex].GetPosition().X - proteinCentre.X, atoms[overlappedIndex].GetPosition().Y - proteinCentre.Y, atoms[overlappedIndex].GetPosition().Z - proteinCentre.Z);

								this->SpawnCylinder(atomPos1 * simulationScale, atomPos2 * simulationScale);
							}
						}
					}
				}
			}
		}
	}

	connectionCount = connectionNum;
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::FromInt(connectionCount));
	//UE_LOG(LogTemp, Warning, TEXT("connections: %d"), connectionCount);
}

void AMolecule::SpawnCylinder(FVector atomPos1, FVector atomPos2) {	
	FVector position = FVector((atomPos1.X + atomPos2.X) / 2, (atomPos1.Y + atomPos2.Y) / 2, (atomPos1.Z + atomPos2.Z) / 2);

	FRotator rotation = UKismetMathLibrary::FindLookAtRotation(position, atomPos2);
	rotation = FRotator(rotation.Pitch + 90, rotation.Yaw, rotation.Roll);

	double thickness = connectionThickness / 10;
	//double length = FVector::Distance(atomPos1/simulationScale, atomPos2/simulationScale);
	double length = FVector::Distance(atomPos1, atomPos2) / simulationScale;
	FVector scale = FVector( thickness, thickness, length/4);

	FTransform transform = FTransform(rotation, position, scale);
	
	if (cylinderMeshPointer != nullptr) {
		cylinderMeshPointer->InstanceConnection(transform);
	}
}

void AMolecule::SetProteinCentre() {
	FVector sumVector = FVector(0, 0, 0);

	for (Atom atom : atoms) {
		FVector atomPos = atom.GetPosition();
		sumVector = FVector((sumVector.X + atomPos.X), (sumVector.Y + atomPos.Y), (sumVector.Z + atomPos.Z));
	}

	int atomNum = atoms.Num();

	sumVector = FVector(sumVector.X / atomNum, sumVector.Y / atomNum, sumVector.Z / atomNum);

	proteinCentre = sumVector;
}





