// Fill out your copyright notice in the Description page of Project Settings.

#include "LevelHandler.h"
#include "SaloonBuilding.h"
#include "NationalBank.h"
#include "Hotel.h"
#include "GeneralStore.h"
#include "MyCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"
#include "Runtime/Engine/Public/EngineUtils.h"


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
			SpawnBankEnemy(1);
		}
	}
}

