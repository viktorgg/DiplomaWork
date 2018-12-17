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

	Health = 100;
	CharacterSpeed = 350.0f;
	PistolFireRate = 0.5;
	bHaveRifle = true;

	DistanceToWalk = 700.0f;

	static ConstructorHelpers::FClassFinder<ARevolver>
		PistolBP(TEXT("Blueprint'/Game/Blueprints/RevolverBP.RevolverBP_C'"));
	if (PistolBP.Succeeded() == true) {
		PistolClass = (UClass*)PistolBP.Class;
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Pistol Not Found!")));
	}
}

void AGroundEnemy::BeginPlay()
{
	Super::BeginPlay();

	FActorSpawnParameters ActorSpawnParams;
	ARevolver* SpawnedPistol = GetWorld()->SpawnActor<ARevolver>(PistolClass, GetActorLocation(), GetActorRotation(), ActorSpawnParams);

}

void AGroundEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MoveForward(NULL);
	Fire();
	// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("%f"), PistolFireRate));
}

void AGroundEnemy::MoveForward(float Input)
{
	Rotate(LineTrace());

	if (GetDistanceToMain() > DistanceToWalk) {
		ForwardInput = 1.0f;
		FVector CurrLoc = GetActorLocation();
		FVector NewLoc = CurrLoc + (GetActorForwardVector() * CharacterSpeed * GetWorld()->GetDeltaSeconds());
		SetActorLocation(NewLoc);
	}
	else {
		ForwardInput = 0.0f;
	}
}

void AGroundEnemy::Fire()
{
	if ((PistolActor != NULL) && (bHavePistol == true) && (bCanFirePistol == true)) {
		PistolActor->SpawnProjectile();
		bCanFirePistol = false;
		GetWorldTimerManager().SetTimer(PistolFireRateHandle, this, &AGroundEnemy::ResetPistolFire, PistolFireRate, false, PistolFireRate);
	}
}

void AGroundEnemy::ResetPistolFire()
{
	GetWorldTimerManager().ClearTimer(PistolFireRateHandle);
	bCanFirePistol = true;
}

float AGroundEnemy::GetDistanceToMain()
{
	FVector DistanceVector = GetActorLocation() - MainCharacterActor->GetActorLocation();
	float Distance = DistanceVector.Size();

	return Distance;
}

void AGroundEnemy::RotateToCharacter()
{
	SetActorRotation(FMath::RInterpConstantTo(GetActorRotation(), LookAtChar(), GetWorld()->DeltaTimeSeconds, 40.0f));
}

void AGroundEnemy::Rotate(float Direction)
{
	if (Direction != 0.0f) {
		AddActorWorldRotation(FRotator(0.0f, 300.0f * Direction * GetWorld()->DeltaTimeSeconds, 0.0f));
	}
	else {
		RotateToCharacter();
	}
}

float AGroundEnemy::LineTrace()
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
	// Go Right
	if (DistanceLeft < DistanceRight) {
		return -1.0f;
	}
	// Go Left
	else if (DistanceRight < DistanceLeft) {
		return 1.0f;
	}
	else {
		return 0.0f;
	}
}
