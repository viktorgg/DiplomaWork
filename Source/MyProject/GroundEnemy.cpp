// Fill out your copyright notice in the Description page of Project Settings.

#include "GroundEnemy.h"
#include "MyCharacter.h"
#include "Revolver.h"
#include "Engine/World.h"
#include "Engine/GameEngine.h"
#include "Runtime/Engine/Public/EngineUtils.h"
#include "DrawDebugHelpers.h"
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
	if (LineTrace() == 1) {
		Rotate(-1.0f);
	}
	if (LineTrace() == 2) {
		Rotate(1.0f);
	}
	if (LineTrace() == 0) {
		RotateToCharacter();
	
	}
	
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

void AGroundEnemy::RotateToCharacter()
{
	SetActorRotation(FMath::RInterpConstantTo(GetActorRotation(), LookAtRot(), GetWorld()->DeltaTimeSeconds, 40.0f));
}

void AGroundEnemy::Rotate(float Input)
{
	AddActorWorldRotation(FRotator(0.0f, 300.0f * Input * GetWorld()->DeltaTimeSeconds, 0.0f));
}

int32 AGroundEnemy::LineTrace()
{
	FHitResult OutHitFront, OutHitFrontL, OutHitFrontR;

	FVector StartLocFront = GetActorLocation();
	FVector EndLocFront = (GetActorForwardVector() * 500.0f) + StartLocFront;

	FVector StartLocFrontL = (GetActorRightVector() * -1.0f * 75.0f) + StartLocFront;
	FVector EndLocFrontL = (GetActorForwardVector() * 500.0f) + StartLocFrontL;

	FVector StartLocFrontR = (GetActorRightVector() * 75.0f) + StartLocFront;
	FVector EndLocFrontR = (GetActorForwardVector() * 500.0f) + StartLocFrontR;

	float DistanceLeft = 0.0f;
	float DistanceRight = 0.0f;

	FCollisionQueryParams CollisionParams;

	GetWorld()->LineTraceSingleByChannel(OutHitFront, StartLocFront, EndLocFront, ECC_Camera, CollisionParams);
	GetWorld()->LineTraceSingleByChannel(OutHitFrontL, StartLocFrontL, EndLocFrontL, ECC_Camera, CollisionParams);
	GetWorld()->LineTraceSingleByChannel(OutHitFrontR, StartLocFrontR, EndLocFrontR, ECC_Camera, CollisionParams);

	//DrawDebugLine(GetWorld(), StartLocFront, EndLocFront, FColor::Emerald, true, 0.5f, 0, 10);
	//DrawDebugLine(GetWorld(), StartLocFrontL, EndLocFrontL, FColor::Emerald, true, 0.5f, 0, 10);
	//DrawDebugLine(GetWorld(), StartLocFrontR, EndLocFrontR, FColor::Emerald, true, 0.5f, 0, 10);

	if (OutHitFront.bBlockingHit == true || OutHitFrontL.bBlockingHit == true || OutHitFrontR.bBlockingHit == true) {
		
		GetWorld()->LineTraceSingleByChannel(OutHitFrontL, StartLocFrontL, EndLocFrontL, ECC_Camera, CollisionParams);

		if (OutHitFrontL.bBlockingHit == true) {

			DistanceLeft = OutHitFrontL.Distance;
			
		}
		else {
			DistanceLeft = 0.0f;
		}
		GetWorld()->LineTraceSingleByChannel(OutHitFrontR, StartLocFrontR, EndLocFrontR, ECC_Camera, CollisionParams);

		if (OutHitFrontR.bBlockingHit == true) {

			DistanceRight = OutHitFrontR.Distance;
			
		}
		else {
			DistanceRight = 0.0f;
		}
	}

	if (DistanceLeft < DistanceRight) {
		return 1;
	}
	else if (DistanceRight < DistanceLeft) {
		return 2;
	}
	else {
		return 0;
	}
}
