// Fill out your copyright notice in the Description page of Project Settings.

#include "SaloonGroundEnemy.h"
#include "Revolver.h"
#include "MyProjectGameInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/World.h"
#include "Engine/GameEngine.h"
#include "Runtime/Engine/Public/TimerManager.h"
#include "Runtime/Engine/Classes/Sound/SoundCue.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Public/TimerManager.h"
#include "Kismet/GameplayStatics.h"


ASaloonGroundEnemy::ASaloonGroundEnemy()
{
	CharacterMovement->MaxWalkSpeed = 275.f;

	bIsKicking = true;

	// Find the DoorKick cue in content browser by reference
	static ConstructorHelpers::FObjectFinder<USoundCue>
		CueAsset(TEXT("SoundCue'/Game/Assets/Sound/FullDoorKickCue.FullDoorKickCue'"));
	if (CueAsset.Succeeded() == true) {
		DoorKick = CueAsset.Object;
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("DoorKick Cue Not Found In SGEnemy!")));
	}
}

void ASaloonGroundEnemy::BeginPlay()
{
	Super::BeginPlay();

	// Kicking animation will stop after 1.5 seconds
	GetWorldTimerManager().SetTimer(KickingHandle, this, &ASaloonGroundEnemy::ResetKicking, 1.5f, false, 1.5f);
	// Play sound halfway through animation
	GetWorldTimerManager().SetTimer(HalfwayKickHandle, this, &ASaloonGroundEnemy::PlaySound, 0.75f, false, 0.75f);
}

void ASaloonGroundEnemy::ResetKicking()
{
	GetWorldTimerManager().ClearTimer(KickingHandle);
	bIsKicking = false;
	CharacterMovement->MaxWalkSpeed = 400.0f;
}

void ASaloonGroundEnemy::PlaySound()
{
	float VolumeControl = Cast<UMyProjectGameInstance>(GetWorld()->GetGameInstance())->VolumeControl;
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), DoorKick, GetActorLocation(), VolumeControl, DoorKick->GetPitchMultiplier());
}

void ASaloonGroundEnemy::MoveForward(float Input)
{
	if (!bIsKicking) {
		Super::MoveForward(NULL);
	}
	else {
		AddActorWorldOffset(GetActorForwardVector() * CharacterMovement->MaxWalkSpeed * GetWorld()->GetDeltaSeconds());
	}
}

void ASaloonGroundEnemy::DestroyChar()
{
	GetWorldTimerManager().ClearTimer(DestroyHandle);

	EnemyHandler->EnemyActor = nullptr;

	Destroy();
}