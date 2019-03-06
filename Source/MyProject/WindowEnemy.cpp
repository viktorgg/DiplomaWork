// Fill out your copyright notice in the Description page of Project Settings.

#include "WindowEnemy.h"
#include "MyCharacter.h"
#include "Rifle.h"
#include "Windows.h"
#include "LevelHandler.h"
#include "BuildingBase.h"
#include "MyProjectGameInstance.h"
#include "Engine/World.h"
#include "Engine/GameEngine.h"
#include "Runtime/Engine/Public/EngineUtils.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "Runtime/Engine/Classes/Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "Runtime/Engine/Public/TimerManager.h"


AWindowEnemy::AWindowEnemy() {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Health = 150.f;
	RifleFireRate = FMath::RandRange(2.0f, 3.5f);
	bHavePistol = true;

	WindowsPlace = 3;

	// Find the rifle class in content browser
	static ConstructorHelpers::FClassFinder<ARifle>
		RifleBP(TEXT("Blueprint'/Game/Blueprints/RifleBP.RifleBP_C'"));
	if (RifleBP.Succeeded() == true) {
		RifleClass = (UClass*)RifleBP.Class;
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Rifle Not Found In WEnemy!")));
	}

	// Find the Scream cue in content browser by reference
	static ConstructorHelpers::FObjectFinder<USoundCue>
		CueAsset(TEXT("SoundCue'/Game/Assets/Sound/ScreamCue.ScreamCue'"));
	if (CueAsset.Succeeded() == true) {
		Scream = CueAsset.Object;
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Scream Cue Not Found In WEnemy!")));
	}
}

void AWindowEnemy::BeginPlay()
{
	Super::BeginPlay();

	FActorSpawnParameters ActorSpawnParams;
	ARifle* SpawnedRifle = GetWorld()->SpawnActor<ARifle>(RifleClass, GetActorLocation(), GetActorRotation(), ActorSpawnParams);

	// Enemy can't fire half a second after spawning
	GetWorldTimerManager().SetTimer(FireDelayHandle, this, &AWindowEnemy::ResetFireDelay, 0.5f, false, 0.5f);

}

void AWindowEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Health > 0) {
		if (bCanFire == true) {
			Fire();
		}
		RotateToCharacter();
	}
}

void AWindowEnemy::Fire()
{
	if ((RifleActor != nullptr) && (bHaveRifle == true) && (bCanFireRifle == true)) {
		RifleActor->SpawnProjectile();
		bCanFireRifle = false;
		GetWorldTimerManager().SetTimer(RifleFireRateHandle, this, &AWindowEnemy::ResetRifleFire, RifleFireRate, false, RifleFireRate);
	}
}

void AWindowEnemy::ResetRifleFire()
{
	GetWorldTimerManager().ClearTimer(RifleFireRateHandle);
	bCanFireRifle = true;
}

void AWindowEnemy::RotateToCharacter()
{
	// If window is on second floor the enemy will be more leaned towards ground
	if (WindowsPlace < 2) {	
		SetActorRotation(FRotator(-40.0f, LookAtChar().Yaw, 0.0f));
	}
	else {
		SetActorRotation(FRotator(-20.0f, LookAtChar().Yaw, 0.0f));
	}
}

void AWindowEnemy::DestroyChar()
{
	GetWorldTimerManager().ClearTimer(DestroyHandle);

	EnemyHandler->SetEnemyActor(nullptr);

	if (EnemyHandler->GetWindowsActor() != nullptr) {
		EnemyHandler->GetWindowsActor()->Close();

		float VolumeControl = Cast<UMyProjectGameInstance>(GetWorld()->GetGameInstance())->VolumeControl;
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), WindowSqueak, EnemyHandler->GetWindowsActor()->GetActorLocation(), VolumeControl, WindowSqueak->GetPitchMultiplier());
	}

	LevelHandlerActor->WEnemyHandler();

	Destroy();
}

void AWindowEnemy::DestroyAfterTime()
{
	GetWorldTimerManager().SetTimer(DestroyHandle, this, &AWindowEnemy::DestroyChar, 1, false, 1);
	if (FMath::RandRange(0, 100) <= 20) {

		float VolumeControl = Cast<UMyProjectGameInstance>(GetWorld()->GetGameInstance())->VolumeControl;
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), Scream, GetActorLocation(), VolumeControl, Scream->GetPitchMultiplier());
	}
}

