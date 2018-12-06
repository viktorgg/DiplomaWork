// Fill out your copyright notice in the Description page of Project Settings.

#include "CharacterBase.h"
#include "Revolver.h"
#include "Rifle.h"
#include "MyCharacter.h"
#include "GroundEnemy.h"
#include "WindowEnemy.h"
#include "Components/CapsuleComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Runtime/Engine/Public/EngineUtils.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "Camera/CameraComponent.h"


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
	PistolFireRate = 0.25f;

	bCanFireRifle = true;
	RifleFireRate = 1.0f;

	bCanRifleAnim = true;

	RootComponent = GetCapsuleComponent();

	GetArrowComponent()->SetupAttachment(RootComponent);

	GetMesh()->SetupAttachment(RootComponent);
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

FRotator ACharacterBase::LookAtChar()
{
	FRotator LookAtChar = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), MainCharacterActor->GetActorLocation());
	return LookAtChar;
}

// Called every frame
void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Health <= 0) {
		Destroy();
		if (Cast<AGroundEnemy>(this) != NULL) {
			PistolActor->GetSphereCollision()->SetSimulatePhysics(true);
		}
		else if(Cast<AWindowEnemy>(this) != NULL){
			RifleActor->GetSphereCollision()->SetSimulatePhysics(true);
		}
	}
}

// Called to bind functionality to input
void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

