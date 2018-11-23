// Fill out your copyright notice in the Description page of Project Settings.

#include "GroundEnemy.h"
#include "MyCharacter.h"
#include "Revolver.h"
#include "Engine/World.h"
#include "Engine/GameEngine.h"
#include "Runtime/Engine/Public/EngineUtils.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Public/TimerManager.h"


AGroundEnemy::AGroundEnemy() {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SetHealth(100);
	SetCharacterSpeed(400.0f);

	DistanceToWalk = 700.0f;
	FireRate = 1.0f;

	static ConstructorHelpers::FClassFinder<ARevolver>
		PistolBP(TEXT("Blueprint'/Game/Blueprints/RevolverBP.RevolverBP_C'"));
	if (PistolBP.Succeeded() == true) {
		PistolRef = (UClass*)PistolBP.Class;
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Pistol Not Found!")));
	}
}

void AGroundEnemy::BeginPlay()
{
	Super::BeginPlay();

	for (TActorIterator<AMyCharacter> ActorItr(GetWorld()); ActorItr; ++ActorItr) {
		if (ActorItr) {
			MainCharacterRef = *ActorItr;
		}
	}
	FActorSpawnParameters ActorSpawnParams;
	ARevolver* SpawnedPistol = GetWorld()->SpawnActor<ARevolver>(PistolRef, GetActorLocation(), GetActorRotation(), ActorSpawnParams);
	SpawnedPistol->SetCharacterRef(this);
	SpawnedPistolRef = SpawnedPistol;
}

void AGroundEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MoveForward(NULL);
	Fire();
}

void AGroundEnemy::MoveForward(float Input)
{
	LerpRotToCharacter(LookAtRot());

	if (GetDistanceToMain() > DistanceToWalk) {
		SetForwardInput(1.0f);
		FVector CurrLoc = GetActorLocation();
		FVector NewLoc = CurrLoc + (GetActorForwardVector() * GetCharacterSpeed() * GetWorld()->GetDeltaSeconds());
		SetActorLocation(NewLoc);
	}
	else {
		SetForwardInput(0.0f);
	}
}

void AGroundEnemy::Fire()
{
	if (PistolRef != NULL && GetCanFirePistol() == true) {

		if (SpawnedPistolRef != NULL) {
			SpawnedPistolRef->SpawnProjectile();
			SetCanFirePistol(false);
			GetWorldTimerManager().SetTimer(GetPistolFireRateHandle(), this, &AGroundEnemy::ResetPistolFire, FireRate, false, FireRate);
		}
		else {
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Error!")));
		}
	}
}

void AGroundEnemy::ResetPistolFire()
{
	GetWorldTimerManager().ClearTimer(GetPistolFireRateHandle());
	SetCanFirePistol(true);
}

float AGroundEnemy::GetDistanceToMain()
{
	FVector DistanceVector = GetActorLocation() - MainCharacterRef->GetActorLocation();
	float Distance = DistanceVector.Size();

	return Distance;
}

FRotator AGroundEnemy::LookAtRot()
{
	FRotator LookAtRot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), MainCharacterRef->GetActorLocation());
	return LookAtRot;
}

void AGroundEnemy::LerpRotToCharacter(FRotator NewRot)
{
	SetActorRotation(FMath::Lerp(GetActorRotation(), NewRot, 5 * GetWorld()->GetDeltaSeconds()));
}
