// Fill out your copyright notice in the Description page of Project Settings.

#include "SaloonGroundEnemy.h"
#include "Revolver.h"
#include "Engine/World.h"
#include "Runtime/Engine/Public/TimerManager.h"


ASaloonGroundEnemy::ASaloonGroundEnemy() 
{
	bIsKicking = true;
	CharacterSpeed = 250.0f;	// Enemy will go slower when kicking door
}

void ASaloonGroundEnemy::BeginPlay()
{
	Super::BeginPlay();

	// Kicking animation will stop after 1.5 seconds
	GetWorldTimerManager().SetTimer(KickingHandle, this, &ASaloonGroundEnemy::ResetKicking, 1.5f, false, 1.5f);
}

void ASaloonGroundEnemy::ResetKicking()
{
	GetWorldTimerManager().ClearTimer(KickingHandle);
	bIsKicking = false;
	CharacterSpeed = 350.0f;
}

void ASaloonGroundEnemy::MoveForward(float Input)
{
	if (bIsKicking == false) {

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
	else {
		FVector CurrLoc = GetActorLocation();
		FVector NewLoc = CurrLoc + (GetActorForwardVector() * CharacterSpeed * GetWorld()->GetDeltaSeconds());
		SetActorLocation(NewLoc);
	}
}
