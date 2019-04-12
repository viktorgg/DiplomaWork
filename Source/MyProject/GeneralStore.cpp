// Fill out your copyright notice in the Description page of Project Settings.

#include "GeneralStore.h"
#include "MyCharacter.h"
#include "Windows.h"
#include "WindowEnemy.h"
#include "Runtime/Engine/Public/EngineUtils.h"
#include "Engine/GameEngine.h"
#include "Engine/World.h"


// Sets default values
AGeneralStore::AGeneralStore() {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AGeneralStore::BeginPlay()
{
	Super::BeginPlay();

}

void AGeneralStore::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// Set the array in MyCharacter for memory deallocation
	for (TActorIterator<AMyCharacter> ActorItr(GetWorld()); ActorItr; ++ActorItr) {
		if (ActorItr) {
			ActorItr->BankEnemyHandlerArray = EnemyHandlerArray;
		}
	}
}

void AGeneralStore::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



