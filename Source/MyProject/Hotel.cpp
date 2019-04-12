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
	if (!EnemyHandlerArray[Place]->GetEnemyActor()) {

		if ((Place < 4) && (EnemyHandlerArray[Place]->GetWindowsActor()->GetIsClosed())) {
			
			EnemyHandlerArray[Place]->GetWindowsActor()->Open();
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), WindowSqueak, EnemyHandlerArray[Place]->GetWindowsActor()->GetActorLocation(), WindowSqueak->GetVolumeMultiplier(), WindowSqueak->GetPitchMultiplier());

			FVector LocOffset;

			// Configure spawn location and rotation depending on floor
			if (Place < 2) {
				LocOffset = (EnemyHandlerArray[Place]->GetWindowsActor()->GetActorRightVector() * -25.0f) + (EnemyHandlerArray[Place]->GetWindowsActor()->GetActorUpVector() * 10)
					+ (EnemyHandlerArray[Place]->GetWindowsActor()->GetActorForwardVector() * 10.0f);
			}
			else {
				LocOffset = (EnemyHandlerArray[Place]->GetWindowsActor()->GetActorRightVector() * -35.0f) + (EnemyHandlerArray[Place]->GetWindowsActor()->GetActorUpVector() * 10)
					+ (EnemyHandlerArray[Place]->GetWindowsActor()->GetActorForwardVector() * 10.0f);
			}
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			AWindowEnemy* SpawnedEnemy = GetWorld()->SpawnActor<AWindowEnemy>(WindowEnemyClass, EnemyHandlerArray[Place]->GetWindowsActor()->GetActorLocation() + LocOffset, GetActorRotation(), ActorSpawnParams);

			SpawnedEnemy->SetWindowsPlace(Place);
			SpawnedEnemy->SetEntryRotation(GetActorRotation());
			SpawnedEnemy->SetBuildingActor(this);
			SpawnedEnemy->SetEnemyHandler(EnemyHandlerArray[Place]);
			SpawnedEnemy->SetWindowSqueak(WindowSqueak);
			EnemyHandlerArray[Place]->SetEnemyActor(SpawnedEnemy);
		}
		else {
			// Spawn enemy at the terrace location
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
			AWindowEnemy* SpawnedEnemy = GetWorld()->SpawnActor<AWindowEnemy>(WindowEnemyClass, EnemyHandlerArray[Place]->GetTerraceLoc(), GetActorRotation(), ActorSpawnParams);

			SpawnedEnemy->SetWindowsPlace(Place);
			SpawnedEnemy->SetEnemyHandler(EnemyHandlerArray[Place]);
			EnemyHandlerArray[Place]->SetEnemyActor(SpawnedEnemy);
		}
	}
}

