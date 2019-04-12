// Fill out your copyright notice in the Description page of Project Settings.

#include "LevelHandler.h"
#include "SaloonBuilding.h"
#include "NationalBank.h"
#include "GroundEnemy.h"
#include "Hotel.h"
#include "GeneralStore.h"
#include "MyCharacter.h"
#include "Windows.h"
#include "MyProjectGameInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/World.h"
#include "Engine/GameEngine.h"
#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"
#include "Runtime/Engine/Public/TimerManager.h"
#include "Runtime/Engine/Public/EngineUtils.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"



// Sets default values
ALevelHandler::ALevelHandler()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bEntered = false;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	BoxCollision->SetupAttachment(RootComponent);

	BoxCollisionBarn = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision Barn"));
	BoxCollisionBarn->SetupAttachment(RootComponent);

	BoxCollisionEnd = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision End"));
	BoxCollisionEnd->SetupAttachment(RootComponent);

	MainCharacter = nullptr;
	GroundEnemy = nullptr;

	SaloonBuildingActor = nullptr;
	SaloonBuildingActor2 = nullptr;
	NationalBankActor = nullptr;
	HotelActor = nullptr;
	GeneralStoreActor = nullptr;

	static ConstructorHelpers::FClassFinder<AGroundEnemy>
		GroundEnemyBP(TEXT("Blueprint'/Game/Blueprints/GroundEnemyBP.GroundEnemyBP_C'"));
	if (GroundEnemyBP.Succeeded() == true) {
		GroundEnemyClass = (UClass*)GroundEnemyBP.Class;
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Ground Enemy Not Found In Level!")));
	}

	static ConstructorHelpers::FClassFinder<ASaloonBuilding>
		SaloonBP(TEXT("Blueprint'/Game/Blueprints/SaloonBP.SaloonBP_C'"));
	if (SaloonBP.Succeeded() == true) {
		SaloonBuildingClass = (UClass*)SaloonBP.Class;
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Saloon Not Found In Level!")));
	}
}

// Called when the game starts or when spawned
void ALevelHandler::BeginPlay()
{
	Super::BeginPlay();
	
	// Calls OnEnterBox
	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &ALevelHandler::OnEnterBox);

	// Get Character from level
	for (TActorIterator<AMyCharacter> ActorItr(GetWorld()); ActorItr; ++ActorItr) {
		if (ActorItr) {
			MainCharacter = *ActorItr;
		}
	}
	// Get Saloon from level
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), SaloonBuildingClass, SaloonArray);
	SaloonArray.Sort();
	SaloonBuildingActor = Cast<ASaloonBuilding>(SaloonArray[1]);
	SaloonBuildingActor->SetSaloonIndex(1);
	SaloonBuildingActor2 = Cast<ASaloonBuilding>(SaloonArray[0]);
	SaloonBuildingActor2->SetSaloonIndex(2);

	// Get Bank from level
	for (TActorIterator<ANationalBank> ActorItr(GetWorld()); ActorItr; ++ActorItr) {
		if (ActorItr) {
			NationalBankActor = *ActorItr;
		}
	}
	// Get Hotel from level
	for (TActorIterator<AHotel> ActorItr(GetWorld()); ActorItr; ++ActorItr) {
		if (ActorItr) {
			HotelActor = *ActorItr;
		}
	}
	// Get Store from level
	for (TActorIterator<AGeneralStore> ActorItr(GetWorld()); ActorItr; ++ActorItr) {
		if (ActorItr) {
			GeneralStoreActor = *ActorItr;
		}
	}
}

// Called every frame
void ALevelHandler::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("%d"), NationalBankActor->GetEnemyHandlerArray()[1]->GetWindowsActor()->GetIsClosed()));
}

void ALevelHandler::SpawnGroundEnemy(int32 Place)
{
	FVector SpawnLoc;
	FRotator SpawnRot;

	bool bCanSpawn = true;
	
	if (Place == Start) {
		SpawnLoc = GEnemyStart;
		SpawnRot = FRotator(0.f, 90.f, 0.f);

		if (IfEnemyInSight(SpawnLoc)) {
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
			AGroundEnemy* SpawnedEnemy = GetWorld()->SpawnActor<AGroundEnemy>(GroundEnemyClass, SpawnLoc, SpawnRot, ActorSpawnParams);
		}
		else {
			bCanSpawn = false;
		}
	}
	else if (Place == Barn) {
		SpawnLoc = GEnemyBarn;
		SpawnRot = FRotator(0.f, 0.f, 0.f);

		if (BoxCollisionBarn->IsOverlappingActor(MainCharacter)) {
			if (IfEnemyInSight(SpawnLoc)) {
				FActorSpawnParameters ActorSpawnParams;
				ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
				AGroundEnemy* SpawnedEnemy = GetWorld()->SpawnActor<AGroundEnemy>(GroundEnemyClass, SpawnLoc, SpawnRot, ActorSpawnParams);
			}
			else {
				bCanSpawn = false;
			}
		}
		else {
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
			AGroundEnemy* SpawnedEnemy = GetWorld()->SpawnActor<AGroundEnemy>(GroundEnemyClass, SpawnLoc, SpawnRot, ActorSpawnParams);
		}
	}
	else if (Place == End) {
		SpawnLoc = GEnemyEnd;
		SpawnRot = FRotator(0.f, 180.f, 0.f);

		if (BoxCollisionEnd->IsOverlappingActor(MainCharacter)) {
			if (IfEnemyInSight(SpawnLoc)) {
				FActorSpawnParameters ActorSpawnParams;
				ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
				AGroundEnemy* SpawnedEnemy = GetWorld()->SpawnActor<AGroundEnemy>(GroundEnemyClass, SpawnLoc, SpawnRot, ActorSpawnParams);
			}
			else {
				bCanSpawn = false;
			}
		}
		else {
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
			AGroundEnemy* SpawnedEnemy = GetWorld()->SpawnActor<AGroundEnemy>(GroundEnemyClass, SpawnLoc, SpawnRot, ActorSpawnParams);
		}
	}
	// If unable to spawn enemy first try, loop until succeeds 
	if (!bCanSpawn) {
		SpawnGroundEnemy(FMath::RandRange(Start, End));
	}
}

void ALevelHandler::GEnemyHandler()
{
	FTimerDelegate GroundEnemyDel;
	FTimerHandle GroundEnemyHandle;

	GroundEnemyDel.BindUFunction(this, FName("SpawnGroundEnemy"), FMath::RandRange(Start, End));

	if (MainCharacter->GetHealth() < 150) {
		GetWorldTimerManager().SetTimer(GroundEnemyHandle, GroundEnemyDel, 10.0f, false, 10.0f);
	}
	else {
		GetWorldTimerManager().SetTimer(GroundEnemyHandle, GroundEnemyDel, 5.0f, false, 5.0f);
	}
}

void ALevelHandler::WEnemyHandler()
{
	FTimerDelegate WEnemyDel;
	FTimerHandle WEnemyHandle;

	int32 Place = FMath::RandRange(Bank, Saloon2);

	if (Place == Bank) {

		WEnemyDel.BindUFunction(this, FName("SpawnBankEnemy"), FMath::RandRange(0, 3));
		if (MainCharacter->GetHealth() < 150) {
			GetWorldTimerManager().SetTimer(WEnemyHandle, WEnemyDel, 10.0f, false, 10.0f);
		}
		else {
			GetWorldTimerManager().SetTimer(WEnemyHandle, WEnemyDel, 5.0f, false, 5.0f);
		}
	}
	else if (Place == Hotel) {

		WEnemyDel.BindUFunction(this, FName("SpawnHotelEnemy"), FMath::RandRange(0, 4));

		if (MainCharacter->GetHealth() < 150) {
			GetWorldTimerManager().SetTimer(WEnemyHandle, WEnemyDel, 10.0f, false, 10.0f);
		}
		else {
			GetWorldTimerManager().SetTimer(WEnemyHandle, WEnemyDel, 5.0f, false, 5.0f);
		}
	}
	else if (Place == Store) {

		WEnemyDel.BindUFunction(this, FName("SpawnStoreEnemy"), FMath::RandRange(0, 3));
		if (MainCharacter->GetHealth() < 150) {
			GetWorldTimerManager().SetTimer(WEnemyHandle, WEnemyDel, 10.0f, false, 10.0f);
		}
		else {
			GetWorldTimerManager().SetTimer(WEnemyHandle, WEnemyDel, 5.0f, false, 5.0f);
		}
	}
	else if (Place == Saloon) {
		
		WEnemyDel.BindUFunction(this, FName("SpawnSaloonEnemy"), FMath::RandRange(0, 2));
		if (MainCharacter->GetHealth() < 150) {
			GetWorldTimerManager().SetTimer(WEnemyHandle, WEnemyDel, 10.0f, false, 10.0f);
		}
		else {
			GetWorldTimerManager().SetTimer(WEnemyHandle, WEnemyDel, 5.0f, false, 5.0f);
		}
	}
	else if (Place == Saloon2) {

		WEnemyDel.BindUFunction(this, FName("SpawnSaloonEnemy2"), FMath::RandRange(0, 2));
		if (MainCharacter->GetHealth() < 150) {
			GetWorldTimerManager().SetTimer(WEnemyHandle, WEnemyDel, 10.0f, false, 10.0f);
		}
		else {
			GetWorldTimerManager().SetTimer(WEnemyHandle, WEnemyDel, 5.0f, false, 5.0f);
		}
	}
}

bool ALevelHandler::IfEnemyInSight(FVector Loc)
{
	FVector CameraForwardVector = MainCharacter->GetCamera()->GetForwardVector();
	CameraForwardVector /= CameraForwardVector.Size();

	// Gets the forward vector of rotation that looks directly at main character
	FVector EnemyForwardVector = UKismetMathLibrary::GetForwardVector(UKismetMathLibrary::FindLookAtRotation(Loc, MainCharacter->GetActorLocation()));
	EnemyForwardVector /= EnemyForwardVector.Size();

	float Angle = FVector::DotProduct(EnemyForwardVector, CameraForwardVector);

	if (Angle > -0.3f) {
		return true;
	}
	else {
		return false;
	}
}

void ALevelHandler::SpawnBankEnemy(int32 Place)
{
	if (!NationalBankActor->GetEnemyHandlerArray()[Place]->GetWindowsActor()->GetIsClosed()) {
		WEnemyHandler();
	}
	else {
		NationalBankActor->SpawnEnemy(Place);
	}
}

void ALevelHandler::SpawnHotelEnemy(int32 Place)
{
	if (Place == 4) {
		if (IfEnemyInSight(HotelActor->GetEnemyHandlerArray()[Place]->GetTerraceLoc())) {
			HotelActor->SpawnEnemy(Place);
		}
	}
	else if (!HotelActor->GetEnemyHandlerArray()[Place]->GetWindowsActor()->GetIsClosed()) {
		WEnemyHandler();
	}
	else {
		HotelActor->SpawnEnemy(Place);
	}
}

void ALevelHandler::SpawnStoreEnemy(int32 Place)
{
	if (!GeneralStoreActor->GetEnemyHandlerArray()[Place]->GetWindowsActor()->GetIsClosed()) {
		WEnemyHandler();
	}
	else {
		GeneralStoreActor->SpawnEnemy(Place);
	}
}

void ALevelHandler::SpawnSaloonEnemy(int32 Place)
{
	if (Place == 1) {
		if (IfEnemyInSight(SaloonBuildingActor->GetSEnemyHandlerArray()[Place]->GetOutLoc())) {
			SaloonBuildingActor->SpawnEnemy(Place);
		}
	}
	else if (SaloonBuildingActor->GetSEnemyHandlerArray()[Place]->GetEnemyActor()) {
		WEnemyHandler();
	}
	else {
		SaloonBuildingActor->SpawnEnemy(Place);
	}
}

void ALevelHandler::SpawnSaloonEnemy2(int32 Place)
{
	if (Place == 1) {
		if (IfEnemyInSight(SaloonBuildingActor2->GetSEnemyHandlerArray()[Place]->GetOutLoc())) {
			SaloonBuildingActor2->SpawnEnemy(Place);
		}
	}
	else if (SaloonBuildingActor2->GetSEnemyHandlerArray()[Place]->GetEnemyActor()) {
		WEnemyHandler();
	}
	else {
		SaloonBuildingActor2->SpawnEnemy(Place);
	}
}

// One time use
void ALevelHandler::OnEnterBox(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!bEntered) {
		if (Cast<AMyCharacter>(OtherActor)) {

			SaloonBuildingActor->SpawnEnemy(2);
			
			FTimerDelegate BankEnemyDel;
			FTimerHandle BankEnemyHandle;

			FTimerDelegate HotelEnemyDel;
			FTimerHandle HotelEnemyHandle;
			
			FTimerDelegate GEnemyDel;
			FTimerHandle GEnemyHandle;

			// Spawn bank enemy just on second floor
			BankEnemyDel.BindUFunction(this, FName("SpawnBankEnemy"), FMath::RandRange(0, 1));
			HotelEnemyDel.BindUFunction(this, FName("SpawnHotelEnemy"), FMath::RandRange(0, 3));
			GEnemyDel.BindUFunction(this, FName("SpawnGroundEnemy"), FMath::RandRange(Start, End));

			GetWorldTimerManager().SetTimer(BankEnemyHandle, BankEnemyDel, 2.0f, false, 2.0f);
			GetWorldTimerManager().SetTimer(HotelEnemyHandle, HotelEnemyDel, 3.0f, false, 3.0f);
			GetWorldTimerManager().SetTimer(GEnemyHandle, GEnemyDel, 7.0f, false, 7.0f);

			// Configure difficulty variables
			if (Cast<UMyProjectGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))->DifficultyAmount == Easy) {
				// Spawns a window enemy every minute
				GetWorldTimerManager().SetTimer(WEnemyHandle, this, &ALevelHandler::WEnemyHandler, 60.0f, true, 7.0f);
			}
			else if (Cast<UMyProjectGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))->DifficultyAmount == Medium) {
				// Spawns a window enemy every half minute
				GetWorldTimerManager().SetTimer(WEnemyHandle, this, &ALevelHandler::WEnemyHandler, 30.0f, true, 7.0f);
			}
			else if (Cast<UMyProjectGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))->DifficultyAmount == Hard) {
				// Spawns a window enemy every quarter minute
				GetWorldTimerManager().SetTimer(WEnemyHandle, this, &ALevelHandler::WEnemyHandler, 15.0f, true, 7.0f);
			}

			// Tries to spawn an enemy at hotel terrace every 20 seconds
			HotelTerraceDel.BindUFunction(this, FName("SpawnHotelEnemy"), 4);
			GetWorldTimerManager().SetTimer(HotelTerraceHandle, HotelTerraceDel, 20.0f, true, 7.0f);

			// Tries to spawn an enemy at saloon terrace every 20 seconds
			SaloonOutDel.BindUFunction(this, FName("SpawnSaloonEnemy"), 1);
			GetWorldTimerManager().SetTimer(SaloonOutHandle, SaloonOutDel, 20.0f, true, 7.0f);

			// Tries to spawn an enemy at saloon terrace every 20 seconds
			SaloonOutDel2.BindUFunction(this, FName("SpawnSaloonEnemy2"), 1);
			GetWorldTimerManager().SetTimer(SaloonOutHandle2, SaloonOutDel2, 20.0f, true, 7.0f);

			bEntered = true;
		}
	}
}




