// Fill out your copyright notice in the Description page of Project Settings.

#include "WindowEnemy.h"
#include "MyCharacter.h"
#include "Rifle.h"
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
	SetActorRotation(FMath::RInterpConstantTo(GetActorRotation(), LookAtChar(), GetWorld()->DeltaTimeSeconds, 40.0f));
}
