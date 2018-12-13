// Fill out your copyright notice in the Description page of Project Settings.

#include "WindowEnemy.h"
#include "MyCharacter.h"
#include "Rifle.h"
#include "Windows.h"
#include "BuildingBase.h"
#include "Engine/World.h"
#include "Engine/GameEngine.h"
#include "Runtime/Engine/Public/EngineUtils.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "Runtime/Engine/Public/TimerManager.h"


AWindowEnemy::AWindowEnemy() {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SetHealth(100);
	SetRifleFireRate(2.0f);

	WindowsPlace = 0;

	static ConstructorHelpers::FClassFinder<ARifle>
		RifleBP(TEXT("Blueprint'/Game/Blueprints/RifleBP.RifleBP_C'"));
	if (RifleBP.Succeeded() == true) {
		RifleClass = (UClass*)RifleBP.Class;
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Rifle Not Found!")));
	}
}

void AWindowEnemy::BeginPlay()
{
	Super::BeginPlay();

	FActorSpawnParameters ActorSpawnParams;
	ARifle* SpawnedRifle = GetWorld()->SpawnActor<ARifle>(RifleClass, GetActorLocation(), GetActorRotation(), ActorSpawnParams);
}

void AWindowEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Fire();
	RotateToCharacter();
	// SetActorRotation(FRotator(GetActorRotation().Pitch, FMath::Clamp(GetActorRotation().Yaw, -40.0f, 40.0f), 0.0f));
	// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Rot: %f"), GetActorRotation().Yaw));
}

void AWindowEnemy::Fire()
{
	if ((GetRifleActor() != NULL) && (GetHaveRifle() == true) && (GetCanFireRifle() == true)) {
		GetRifleActor()->SpawnProjectile();
		SetCanFireRifle(false);
		GetWorldTimerManager().SetTimer(GetRifleFireRateHandle(), this, &AWindowEnemy::ResetRifleFire, GetRifleFireRate(), false, GetRifleFireRate());
	}
}

void AWindowEnemy::ResetRifleFire()
{
	GetWorldTimerManager().ClearTimer(GetRifleFireRateHandle());
	SetCanFireRifle(true);
}

void AWindowEnemy::RotateToCharacter()
{
	if (WindowsPlace < 2) {
		SetActorRotation(FMath::Lerp(FRotator(-40.0f, GetActorRotation().Yaw, 0.0f), FRotator(-40.0f, LookAtChar().Yaw, 0.0f), 15 * GetWorld()->GetDeltaSeconds()));
	}
	else {
		SetActorRotation(FMath::Lerp(FRotator(-20.0f, GetActorRotation().Yaw, 0.0f), FRotator(-20.0f, LookAtChar().Yaw, 0.0f), 15 * GetWorld()->GetDeltaSeconds()));
	}
}

