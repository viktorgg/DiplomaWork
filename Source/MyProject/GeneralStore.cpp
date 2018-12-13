// Fill out your copyright notice in the Description page of Project Settings.

#include "GeneralStore.h"
#include "Windows.h"
#include "WindowEnemy.h"
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

	for (int32 i = 2; i < 4; i++) {
		SpawnEnemy(i);
	}
}

void AGeneralStore::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGeneralStore::SpawnEnemy(int32 Place)
{
	GetWindowsArray()[Place]->Open();
	FVector LocOffset;
	FRotator RotOffset;
	if (Place < 2) {
		LocOffset = (GetWindowsArray()[Place]->GetActorRightVector() * -25.0f) + (GetWindowsArray()[Place]->GetActorUpVector() * 10) + (GetWindowsArray()[Place]->GetActorForwardVector() * 10.0f);
		RotOffset = FRotator(0.0f, 90.0f, 0.0f);
	}
	else {
		LocOffset = (GetWindowsArray()[Place]->GetActorRightVector() * -35.0f) + (GetWindowsArray()[Place]->GetActorUpVector() * 10) + (GetWindowsArray()[Place]->GetActorForwardVector() * 10.0f);
		RotOffset = FRotator(0.0f, 90.0f, 0.0f);
	}
	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	AWindowEnemy* SpawnedEnemy = GetWorld()->SpawnActor<AWindowEnemy>(GetWindowEnemyClass(), GetWindowsArray()[Place]->GetActorLocation() + LocOffset, GetActorRotation() + RotOffset, ActorSpawnParams);
	SpawnedEnemy->SetWindowsActor(GetWindowsArray()[Place]);
	SpawnedEnemy->SetWindowsPlace(Place);
}


