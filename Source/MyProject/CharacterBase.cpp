// Fill out your copyright notice in the Description page of Project Settings.

#include "CharacterBase.h"
#include "Revolver.h"
#include "Rifle.h"
#include "MyCharacter.h"
#include "GroundEnemy.h"
#include "WindowEnemy.h"
#include "Windows.h"
#include "Components/CapsuleComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Runtime/Engine/Public/EngineUtils.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Animation/AnimationAsset.h"
#include "Runtime/Engine/Public/TimerManager.h"
#include "Engine/GameEngine.h"


// Sets default values
ACharacterBase::ACharacterBase()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Health = 0.0f;
	CharacterSpeed = 0.0f;

	ForwardInput = 0.0f;
	RightInput = 0.0f;

	bHavePistol = false;
	bHaveRifle = false;

	bCanFirePistol = true;
	PistolFireRate = 0.0f;

	bCanFireRifle = true;
	RifleFireRate = 0.0f;

	bCanRifleAnim = true;

	RootComponent = GetCapsuleComponent();

	GetArrowComponent()->SetupAttachment(RootComponent);

	GetMesh()->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UAnimSequence>
		AnimAsset(TEXT("AnimSequence'/Game/Assets/Animations/EnemyChar/Shoulder_Hit_And_Fall.Shoulder_Hit_And_Fall'"));
	if (AnimAsset.Succeeded() == true) {
		DeathAnim = AnimAsset.Object;
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Animation Not Found!")));
	}
}

// Called when the game starts or when spawned
void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();

	for (TActorIterator<AMyCharacter> ActorItr(GetWorld()); ActorItr; ++ActorItr) {
		if (ActorItr) {
			MainCharacterActor = *ActorItr;
		}
	}
}

void ACharacterBase::PlayDeathAnim()
{
	GetMesh()->PlayAnimation(DeathAnim, false);
}

void ACharacterBase::DestroyAfterTime()
{
	GetWorldTimerManager().SetTimer(DestroyHandle, this, &ACharacterBase::DestroyChar, 3, false, 3);
}

void ACharacterBase::DestroyChar()
{
	GetWorldTimerManager().ClearTimer(DestroyHandle);
	Destroy();
}

// Called every frame
void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

FRotator ACharacterBase::LookAtChar()
{
	FRotator LookAtChar = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), MainCharacterActor->GetActorLocation());
	return LookAtChar;
}

