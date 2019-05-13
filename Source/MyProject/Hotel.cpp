// Fill out your copyright notice in the Description page of Project Settings.

#include "Hotel.h"
#include "MyCharacter.h"
#include "Windows.h"
#include "WindowEnemy.h"
#include "Engine/GameEngine.h"
#include "Kismet/GameplayStatics.h"
#include "Runtime/Engine/Public/EngineUtils.h"
#include "Runtime/Engine/Classes/Sound/SoundCue.h"
#include "Engine/World.h"



// Sets default values
AHotel::AHotel() {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AHotel::BeginPlay()
{
	Super::BeginPlay();

}

void AHotel::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// Set the array in MyCharacter for memory deallocation
	for (TActorIterator<AMyCharacter> ActorItr(GetWorld()); ActorItr; ++ActorItr) {
		if (ActorItr) {
			ActorItr->HotelEnemyHandlerArray = EnemyHandlerArray;
		}
	}
}

void AHotel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHotel::SpawnEnemy(int32 Place)
{	
	if (!EnemyHandlerArray[Place]->EnemyActor) {

		if ((Place < 4) && (EnemyHandlerArray[Place]->WindowsActor->GetIsClosed())) {
			
			EnemyHandlerArray[Place]->WindowsActor->Open();
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), WindowSqueak, EnemyHandlerArray[Place]->WindowsActor->GetActorLocation(), WindowSqueak->GetVolumeMultiplier(), WindowSqueak->GetPitchMultiplier());

			FVector LocOffset;

			// Configure spawn location and rotation depending on floor
			if (Place < 2) {
				LocOffset = (EnemyHandlerArray[Place]->WindowsActor->GetActorRightVector() * -25.0f) + (EnemyHandlerArray[Place]->WindowsActor->GetActorUpVector() * 10)
					+ (EnemyHandlerArray[Place]->WindowsActor->GetActorForwardVector() * 10.0f);
			}
			else {
				LocOffset = (EnemyHandlerArray[Place]->WindowsActor->GetActorRightVector() * -35.0f) + (EnemyHandlerArray[Place]->WindowsActor->GetActorUpVector() * 10)
					+ (EnemyHandlerArray[Place]->WindowsActor->GetActorForwardVector() * 10.0f);
			}
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			AWindowEnemy* SpawnedEnemy = GetWorld()->SpawnActor<AWindowEnemy>(WindowEnemyClass, EnemyHandlerArray[Place]->WindowsActor->GetActorLocation() + LocOffset, GetActorRotation(), ActorSpawnParams);

			SpawnedEnemy->SetWindowsPlace(Place);
			SpawnedEnemy->SetEntryRotation(GetActorRotation());
			SpawnedEnemy->SetBuildingActor(this);
			SpawnedEnemy->SetEnemyHandler(EnemyHandlerArray[Place]);
			SpawnedEnemy->SetWindowSqueak(WindowSqueak);
			EnemyHandlerArray[Place]->EnemyActor = SpawnedEnemy;
		}
		else {
			// Spawn enemy at the terrace location
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
			AWindowEnemy* SpawnedEnemy = GetWorld()->SpawnActor<AWindowEnemy>(WindowEnemyClass, EnemyHandlerArray[Place]->TerraceLoc, GetActorRotation(), ActorSpawnParams);

			SpawnedEnemy->SetWindowsPlace(Place);
			SpawnedEnemy->SetEnemyHandler(EnemyHandlerArray[Place]);
			EnemyHandlerArray[Place]->EnemyActor = SpawnedEnemy;
		}
	}
}

