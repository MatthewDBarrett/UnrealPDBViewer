#include "Molecule.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/LineBatchComponent.h"
#include "Engine/World.h"
#include "Components/StaticMeshComponent.h"
#include "Atom.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetSystemLibrary.h"
#include "InstancedStaticMeshActor.h"

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

	FActorSpawnParameters SpawnParams;
	FVector pos = FVector(0, 0, 0);
	FRotator rot = FRotator(0, 0, 0);

	instancedStaticMeshActor = GetWorld()->SpawnActor<AActor>(StaticMeshToSpawn, pos, rot, SpawnParams);

	meshPointer = Cast<AInstancedStaticMeshActor>(instancedStaticMeshActor);

	this->ConvertPDB(moleculeName);
	this->SetAtomSizes();

	this->SpawnAtoms();

	if (renderConnections)
		this->SpawnConnections();
}

// Called every frame
void AMolecule::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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

void AMolecule::SetAtomSizes() {
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

		for (int i = 0; i < atoms.Num(); i++) {
			int32 index = (stringRecords.Find(atoms[i].GetElementSymbol()));
			FString radiusString = stringRecords[index + 2];
			double radiusDouble = FCString::Atod(*radiusString);
			atoms[i].SetRadius((FCString::Atod(*radiusString)) / 0.775f);
			//UE_LOG(LogTemp, Warning, TEXT("radius: %lf"), radiusDouble);
		}
	}
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
	for (Atom atom : atoms) {
		//UE_LOG(LogTemp, Warning, TEXT("atom Name: %s"), *atom.GetElementSymbol());

		FVector position = FVector(atom.GetXPos() * simulationScale, atom.GetYPos() * simulationScale, atom.GetZPos() * simulationScale);
		double size = ( atom.GetRadius() / atomScale ) * simulationScale;

		//this->SpawnSphere(position, size, atom.GetElementSymbol());
		
		FRotator rot = FRotator(0, 0, 0);
		FVector scale = FVector(size, size, size);
		
		FTransform transform = FTransform(rot, position, scale);
		
		if (meshPointer != nullptr) {
			meshPointer->InstanceAtom(transform);
			double randValue = FMath::RandRange(0, 100);
			UE_LOG(LogTemp, Log, TEXT("value1: %d"), randValue);
			double rand = randValue / 100;
			UE_LOG(LogTemp, Log, TEXT("value2: %d"), rand);
			meshPointer->SetCustomData(count, 0, rand, true);
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
	SpawnedActorRef->SetActorScale3D(FVector((size / atomScale) * simulationScale));

	atomName.Append( "-Atom_" );							//String Formatting
	FString CountString = FString::FromInt(atomCount);
	atomName.Append(CountString);

	SpawnedActorRef->SetActorLabel( atomName );					//Setting Label
}

void AMolecule::SpawnConnections() {
	int32 connectionCount = 0;

	TArray<TEnumAsByte<EObjectTypeQuery>> traceObjectTypes;
	traceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
	TArray<AActor*> ignoreActors;
	TArray<AActor*> outActors;
	float searchRadius;
	FVector atomPosition;
	UClass* seekClass = AActor::StaticClass();


	for (Atom atom : atoms) {
		searchRadius = ( atom.GetRadius() * 2 ) * simulationScale;
		//UE_LOG(LogTemp, Log, TEXT("atomRadius: %lf searchRadius: %lf"), atom.GetRadius() ,searchRadius);
		atomPosition = atom.GetPosition() * simulationScale;

		UKismetSystemLibrary::SphereOverlapActors(GetWorld(), atomPosition, searchRadius, traceObjectTypes, seekClass, ignoreActors, outActors);

		for (AActor* overlappedActor : outActors) {
			FString overlappedName = overlappedActor->GetName();
			//UE_LOG(LogTemp, Log, TEXT("OverlappedActor: %s"), *overlappedActor->GetActorTransform().GetScale3D().ToString());
			//UE_LOG(LogTemp, Log, TEXT("OverlappedActor: %s"), meshPointer->setcustom);
			if (overlappedName.Contains("ATOM")) {
				
				FString Left, Right;
				overlappedName.Split(TEXT("_"), &Left, &Right);

				int32 overlappedIndex = (FCString::Atoi(*Right) - 1);

				if (overlappedIndex < atom.GetSerialNum()) {
					if (atom.GetSerialNum() != atoms[overlappedIndex].GetSerialNum()) {				//Check that the same atom is not comparing to itself
						if (this->isConnection(atom, atoms[overlappedIndex])) {
							connectionCount++;
							this->SpawnCylinder(atom.GetPosition() * simulationScale, atoms[overlappedIndex].GetPosition() * simulationScale);
						}
					}
				}
			}
		}
	}


	UE_LOG(LogTemp, Warning, TEXT("connections: %d"), connectionCount);
}

void AMolecule::SpawnCylinder(FVector atomPos1, FVector atomPos2) {
	DrawDebugLine(GetWorld(), atomPos1, atomPos2, FColor::White, true, -1, 0, connectionThickness * simulationScale);
}





