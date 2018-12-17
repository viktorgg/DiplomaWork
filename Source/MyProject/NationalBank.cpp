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
		SpawnEnemy(i);
	}
	
}

void ANationalBank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ANationalBank::SpawnEnemy(int32 Place)
{
	EnemyHandlerArray[Place].GetWindowsActor()->Open();
	FVector LocOffset;
	FRotator RotOffset;
	if (Place < 2) {
		LocOffset = (EnemyHandlerArray[Place].GetWindowsActor()->GetActorRightVector() * -25.0f) + (EnemyHandlerArray[Place].GetWindowsActor()->GetActorUpVector() * 10)
			+ (EnemyHandlerArray[Place].GetWindowsActor()->GetActorForwardVector() * 10.0f);
		RotOffset = FRotator(0.0f, 90.0f, 0.0f);
	}
	else {
		LocOffset = (EnemyHandlerArray[Place].GetWindowsActor()->GetActorRightVector() * -35.0f) + (EnemyHandlerArray[Place].GetWindowsActor()->GetActorUpVector() * 10) 
			+ (EnemyHandlerArray[Place].GetWindowsActor()->GetActorForwardVector() * 10.0f);
		RotOffset = FRotator(0.0f, 90.0f, 0.0f);
	}
	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	AWindowEnemy* SpawnedEnemy = GetWorld()->SpawnActor<AWindowEnemy>(WindowEnemyClass, EnemyHandlerArray[Place].GetWindowsActor()->GetActorLocation() + LocOffset, GetActorRotation() + RotOffset, ActorSpawnParams);
	
	SpawnedEnemy->SetWindowsPlace(Place);
	EnemyHandlerArray[Place].SetWindowEnemyActor(SpawnedEnemy);
}

