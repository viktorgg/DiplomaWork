// Fill out your copyright notice in the Description page of Project Settings.

#include "GroundEnemy.h"
#include "MyCharacter.h"
#include "Revolver.h"
#include "LevelHandler.h"
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

	Health = 100.f;
	CharacterSpeed = 350.f;
	PistolFireRate = FMath::RandRange(0.75f, 1.5f);
	bHaveRifle = true;
	bIsRotating = false;

	DistanceToWalk = 500.f;	// Distance to reach between him and player 
	
	// Find the revolver class in content browser
	static ConstructorHelpers::FClassFinder<ARevolver>
		PistolBP(TEXT("Blueprint'/Game/Blueprints/RevolverBP.RevolverBP_C'"));
	if (PistolBP.Succeeded() == true) {
		PistolClass = (UClass*)PistolBP.Class;
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Pistol Not Found In GEnemy!")));
	}
}

void AGroundEnemy::BeginPlay()
{
	Super::BeginPlay();

	// Spawn revolver so that enemy starts with a weapon
	FActorSpawnParameters ActorSpawnParams;
	ARevolver* SpawnedPistol = GetWorld()->SpawnActor<ARevolver>(PistolClass, GetActorLocation(), GetActorRotation(), ActorSpawnParams);

}

void AGroundEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Health > 0.f) {
		MoveForward(NULL);
		Fire();
	}
	// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("%f"), ForwardInput));
}

void AGroundEnemy::MoveForward(float Input)
{
	if (GetDistanceToMain() >= DistanceToWalk) {
		ForwardInput = 1.f;
		FVector CurrLoc = GetActorLocation();
		FVector NewLoc = CurrLoc + (GetActorForwardVector() * CharacterSpeed * GetWorld()->GetDeltaSeconds());
		SetActorLocation(NewLoc);
	}
	else {
		ForwardInput = 0.f;
	}
	Rotate(LineTrace());
}

void AGroundEnemy::Fire()
{
	if ((PistolActor != nullptr) && (bHavePistol == true) && (bCanFirePistol == true)) {
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
	SetActorRotation(FMath::RInterpConstantTo(GetActorRotation(), LookAtChar(), GetWorld()->DeltaTimeSeconds, 120.f));
}

void AGroundEnemy::DestroyAfterTime()
{
	GetWorldTimerManager().SetTimer(DestroyHandle, this, &AGroundEnemy::DestroyChar, 3, false, 3);
}

void AGroundEnemy::Rotate(float Direction)
{
	// Determine if character should rotate in one place
	if ((MainCharacterActor->GetForwardInput() == 0.f) && (MainCharacterActor->GetRightInput() == 0.f)) {
		bIsRotating = false;
	}
	else {
		bIsRotating = true;
	}
	if (Direction != 0.f) {
		AddActorWorldRotation(FRotator(0.f, 350.f * Direction * GetWorld()->DeltaTimeSeconds, 0.f));
		bIsRotating = true;
	}
	else {
		RotateToCharacter();
	}
}

void AGroundEnemy::DestroyChar()
{
	GetWorldTimerManager().ClearTimer(DestroyHandle);

	EnemyHandler->SetEnemyActor(nullptr);

	LevelHandlerActor->GEnemyHandler();
	// delete EnemyHandler;

	Destroy();
}

// If enemy reaches a wall it returns the nearer direction it needs to rotate in order to go around wall
float AGroundEnemy::LineTrace()
{
	FHitResult OutHitFront, OutHitFrontL, OutHitFrontR;

	FVector StartLocFront = GetActorLocation();
	FVector EndLocFront = (GetActorForwardVector() * 250.f) + StartLocFront;

	FVector StartLocFrontL = (GetActorRightVector() * -1.f * 50.f) + StartLocFront;
	FVector EndLocFrontL = (GetActorForwardVector() * 250.f) + StartLocFrontL;

	FVector StartLocFrontR = (GetActorRightVector() * 50.f) + StartLocFront;
	FVector EndLocFrontR = (GetActorForwardVector() * 250.f) + StartLocFrontR;

	float DistanceLeft = 0.f;
	float DistanceRight = 0.f;

	FCollisionQueryParams CollisionParams;

	GetWorld()->LineTraceSingleByChannel(OutHitFront, StartLocFront, EndLocFront, ECC_Camera, CollisionParams);
	GetWorld()->LineTraceSingleByChannel(OutHitFrontL, StartLocFrontL, EndLocFrontL, ECC_Camera, CollisionParams);
	GetWorld()->LineTraceSingleByChannel(OutHitFrontR, StartLocFrontR, EndLocFrontR, ECC_Camera, CollisionParams);

	//DrawDebugLine(GetWorld(), StartLocFront, EndLocFront, FColor::Emerald, true, 1.f, 0, 10);
	//DrawDebugLine(GetWorld(), StartLocFrontL, EndLocFrontL, FColor::Emerald, true, 1.f, 0, 10);
	//DrawDebugLine(GetWorld(), StartLocFrontR, EndLocFrontR, FColor::Emerald, true, 1.f, 0, 10);

	if (OutHitFront.GetActor() != this || OutHitFrontL.GetActor() != this || OutHitFrontR.GetActor() != this) {
		
		if (OutHitFrontL.GetActor() != this) {
			DistanceLeft = OutHitFrontL.Distance;
		}
		if (OutHitFrontR.GetActor() != this) {
			DistanceRight = OutHitFrontR.Distance;
		}
	}
	// Go Right
	if (DistanceLeft < DistanceRight) {
		return -1.f;
	}
	// Go Left
	else if (DistanceRight < DistanceLeft) {
		return 1.f;
	}
	// Go Straight
	else {
		return 0.f;
	}
}
