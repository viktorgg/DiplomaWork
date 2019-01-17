// Fill out your copyright notice in the Description page of Project Settings.

#include "LevelHandler.h"
#include "SaloonBuilding.h"
#include "NationalBank.h"
#include "GroundEnemy.h"
#include "Hotel.h"
#include "GeneralStore.h"
#include "MyCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/World.h"
#include "Engine/GameEngine.h"
#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"
#include "Runtime/Engine/Public/TimerManager.h"
#include "Runtime/Engine/Public/EngineUtils.h"
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

	static ConstructorHelpers::FClassFinder<AGroundEnemy>
		GroundEnemyBP(TEXT("Blueprint'/Game/Blueprints/GroundEnemyBP.GroundEnemyBP_C'"));
	if (GroundEnemyBP.Succeeded() == true) {
		GroundEnemyClass = (UClass*)GroundEnemyBP.Class;
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Ground Enemy Not Found!")));
	}
}

// Called when the game starts or when spawned
void ALevelHandler::BeginPlay()
{
	Super::BeginPlay();
	
	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &ALevelHandler::OnEnterBox);

	// Get Character from level
	for (TActorIterator<AMyCharacter> ActorItr(GetWorld()); ActorItr; ++ActorItr) {
		if (ActorItr) {
			MainCharacter = *ActorItr;
		}
	}
	// Get Saloon from level
	for (TActorIterator<ASaloonBuilding> ActorItr(GetWorld()); ActorItr; ++ActorItr) {
		if (ActorItr) {
			SaloonBuildingActor = *ActorItr;
		}
	}
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

}

void ALevelHandler::SpawnGroundEnemy(int32 Place)
{
	FVector SpawnLoc;
	FRotator SpawnRot;

	bool bCanSpawn = true;
	

	if (Place == Start) {
		SpawnLoc = GEnemyStart;
		SpawnRot = FRotator(0.f, 90.f, 0.f);

		if (IfEnemyInSight(SpawnLoc, SpawnRot) == true) {
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

		if (BoxCollisionBarn->IsOverlappingActor(MainCharacter) == true) {
			if (IfEnemyInSight(SpawnLoc, SpawnRot) == true) {
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

		if (BoxCollisionEnd->IsOverlappingActor(MainCharacter) == true) {
			if (IfEnemyInSight(SpawnLoc, SpawnRot) == true) {
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
	if (bCanSpawn == false) {
		SpawnGroundEnemy(FMath::RandRange(Start, End));
	}
}

void ALevelHandler::GEnemyHandler()
{
	FTimerDelegate GroundEnemyDel;
	FTimerHandle GroundEnemyHandle;

	GroundEnemyDel.BindUFunction(this, FName("SpawnGroundEnemy"), FMath::RandRange(Start, End));

	if (MainCharacter->GetHealth() < 200) {
		GetWorldTimerManager().SetTimer(GroundEnemyHandle, GroundEnemyDel, 10.0f, false, 10.0f);
	}
	else {
		GetWorldTimerManager().SetTimer(GroundEnemyHandle, GroundEnemyDel, 5.0f, false, 5.0f);
	}
}

void ALevelHandler::WEnemyHandler()
{
	FTimerDelegate GroundEnemyDel;
	FTimerHandle GroundEnemyHandle;

	GroundEnemyDel.BindUFunction(this, FName("SpawnWindowEnemy"), FMath::RandRange(Start, End));

	if (MainCharacter->GetHealth() < 200) {
		GetWorldTimerManager().SetTimer(GroundEnemyHandle, GroundEnemyDel, 10.0f, false, 10.0f);
	}
	else {
		GetWorldTimerManager().SetTimer(GroundEnemyHandle, GroundEnemyDel, 5.0f, false, 5.0f);
	}
}

bool ALevelHandler::IfEnemyInSight(FVector Loc, FRotator Rot)
{
	FVector EnemyForwardVector = UKismetMathLibrary::GetForwardVector(Rot);

	float Angle = FMath::Acos(FVector::DotProduct(EnemyForwardVector, MainCharacter->GetCamera()->GetForwardVector()));
	
	if (Angle <= 1.7f) {
		return true;
	}
	else {
		return false;
	}
}

void ALevelHandler::SpawnBankEnemy(int32 Place)
{
	NationalBankActor->SpawnEnemy(Place);
}

// One time use
void ALevelHandler::SpawnGEnemy()
{
	SpawnGroundEnemy(Barn);
}

// One time use
void ALevelHandler::OnEnterBox(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bEntered == false) {
		if (Cast<AMyCharacter>(OtherActor) != NULL) {
			SaloonBuildingActor->SpawnEnemy(2);
			bEntered = true;

			// Spawn enemy just on second floor
			BankEnemyDel.BindUFunction(this, FName("SpawnBankEnemy"), FMath::RandRange(0, 1));
			GetWorldTimerManager().SetTimer(BankEnemyHandle, BankEnemyDel, 2.0f, false, 2.0f);
			GetWorldTimerManager().SetTimer(GEnemyHandle, this, &ALevelHandler::SpawnGEnemy, 7.0f, false, 7.0f);
		}
	}
}



