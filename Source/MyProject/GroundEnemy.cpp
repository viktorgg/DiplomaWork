// Fill out your copyright notice in the Description page of Project Settings.

#include "GroundEnemy.h"
#include "MyCharacter.h"
#include "Revolver.h"
#include "LevelHandler.h"
#include "GameFramework/CharacterMovementComponent.h"
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

	Health = 150.f;
	CharacterMovement->MaxWalkSpeed = 400.f;
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

	// Enemy can't fire two seconds after spawning
	GetWorldTimerManager().SetTimer(FireDelayHandle, this, &AGroundEnemy::ResetFireDelay, 1.5f, false, 1.5f);
}

void AGroundEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Health > 0.f) {
		MoveForward(NULL);
		if (bCanFire) {
			Fire();
		}
	}
	// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("%f"), Health));
}

void AGroundEnemy::MoveForward(float Input)
{
	if (GetDistanceToMain() >= DistanceToWalk) {
		ForwardInput = 1.f;
		AddActorWorldOffset(GetActorForwardVector() * CharacterMovement->MaxWalkSpeed * GetWorld()->GetDeltaSeconds());
	}
	else {
		ForwardInput = 0.f;
	}
	Rotate(LineTrace());
}

void AGroundEnemy::Fire()
{
	if (PistolActor && bHavePistol && bCanFirePistol) {
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
	// Gets the length of vector
	float Distance = DistanceVector.Size();

	return FMath::Abs(Distance);
}

void AGroundEnemy::RotateToCharacter()
{
	SetActorRotation(FMath::RInterpConstantTo(GetActorRotation(), LookAtChar(), GetWorld()->DeltaTimeSeconds, 120.f));
}

void AGroundEnemy::DestroyAfterTime()
{
	GetWorldTimerManager().SetTimer(DestroyHandle, this, &AGroundEnemy::DestroyChar, 3, false, 3);
}

// Determines if character should rotate in one place
void AGroundEnemy::StationaryRotation()
{
	if ((MainCharacterActor->GetForwardInput() == 0.f) && (MainCharacterActor->GetRightInput() == 0.f)) {
		bIsRotating = false;
	}
	else {
		// Calculate the direction of movement of Main Character
		FVector MainCharDirection = MainCharacterActor->GetVelocity();

		MainCharDirection /= MainCharDirection.Size();
	
		// Calculate angle between the MovementDirection and ActorForwardVector vectors
		float Angle = UKismetMathLibrary::DegAcos(FVector::DotProduct(MainCharDirection, GetActorForwardVector()));
		
		if ((UKismetMathLibrary::Abs(Angle) >= 45.f) && (UKismetMathLibrary::Abs(Angle) <= 135.f)) {
			bIsRotating = true;
		}
	}
}

void AGroundEnemy::Rotate(float Direction)
{
	StationaryRotation();

	if (Direction != 0.f) {
		AddActorWorldRotation(FRotator(0.f, 350.f * Direction * GetWorld()->DeltaTimeSeconds, 0.f));
	}
	else {
		RotateToCharacter();
	}
}

void AGroundEnemy::DestroyChar()
{
	GetWorldTimerManager().ClearTimer(DestroyHandle);

	LevelHandlerActor->GEnemyHandler();

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

	// Start rotating unless Main Char is in front
	if ((OutHitFront.GetActor() != GetMainCharacterActor()) && (OutHitFrontL.GetActor() != GetMainCharacterActor()) && (OutHitFrontR.GetActor() != GetMainCharacterActor())) {
		
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