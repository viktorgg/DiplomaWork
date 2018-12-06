// Fill out your copyright notice in the Description page of Project Settings.

#include "NationalBank.h"
#include "Windows.h"
#include "WindowEnemy.h"
#include "Engine/GameEngine.h"
#include "Engine/World.h"


// Sets default values
ANationalBank::ANationalBank() {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void ANationalBank::BeginPlay()
{
	Super::BeginPlay();

	for (int32 i = 0; i < 2; i++) {
		//SpawnEnemy(i);
	}
}

void ANationalBank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ANationalBank::SpawnEnemy(int32 Place)
{
	GetWindowsArray()[Place]->Open();
	FVector LocOffset = (GetWindowsArray()[Place]->GetActorRightVector() * -50.0f) + (GetWindowsArray()[Place]->GetActorUpVector() * 10);
	FRotator RotOffset = FRotator(0.0f, 90.0f, 0.0f);
	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	AWindowEnemy* SpawnedEnemy = GetWorld()->SpawnActor<AWindowEnemy>(GetWindowEnemyClass(), GetWindowsArray()[Place]->GetActorLocation() + LocOffset, GetActorRotation() +RotOffset, ActorSpawnParams);
}
