// Fill out your copyright notice in the Description page of Project Settings.

#include "LevelHandler.h"
#include "SaloonBuilding.h"
#include "NationalBank.h"
#include "GroundEnemy.h"
#include "Hotel.h"
#include "GeneralStore.h"
#include "MyCharacter.h"
#include "Engine/World.h"
#include "Engine/GameEngine.h"
#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"
#include "Runtime/Engine/Public/TimerManager.h"
#include "Runtime/Engine/Public/EngineUtils.h"
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

	for (TActorIterator<ASaloonBuilding> ActorItr(GetWorld()); ActorItr; ++ActorItr) {
		if (ActorItr) {
			SaloonBuildingActor = *ActorItr;
		}
	}
	for (TActorIterator<ANationalBank> ActorItr(GetWorld()); ActorItr; ++ActorItr) {
		if (ActorItr) {
			NationalBankActor = *ActorItr;
		}
	}
	for (TActorIterator<AHotel> ActorItr(GetWorld()); ActorItr; ++ActorItr) {
		if (ActorItr) {
			HotelActor = *ActorItr;
		}
	}
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

	if (Place == Start) {
		SpawnLoc = GEnemyStart;
		SpawnRot = FRotator(0.f, 90.f, 0.f);
	}
	else if (Place == Barn) {
		SpawnLoc = GEnemyBarn;
		SpawnRot = FRotator(0.f, 0.f, 0.f);
	}
	else if (Place == End) {
		SpawnLoc = GenemyEnd;
		SpawnRot = FRotator(0.f, 180.f, 0.f);
	}
	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	AGroundEnemy* SpawnedEnemy = GetWorld()->SpawnActor<AGroundEnemy>(GroundEnemyClass, SpawnLoc, SpawnRot, ActorSpawnParams);
}

void ALevelHandler::SpawnBankEnemy(int32 Place)
{
	NationalBankActor->SpawnEnemy(Place);
}

void ALevelHandler::OnEnterBox(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bEntered == false) {
		if (Cast<AMyCharacter>(OtherActor) != NULL) {
			SaloonBuildingActor->SpawnEnemy(2);
			bEntered = true;

			BankEnemyDel.BindUFunction(this, FName("SpawnBankEnemy"), FMath::RandRange(0, 1));
			GetWorldTimerManager().SetTimer(BankEnemyHandle, BankEnemyDel, 2.0f, false, 2.0f);
		}
	}
}

