// Fill out your copyright notice in the Description page of Project Settings.

#include "CharacterBase.h"
#include "Revolver.h"
#include "Components/CapsuleComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"


// Sets default values
ACharacterBase::ACharacterBase()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ForwardInput = 0.0f;
	RightInput = 0.0f;

	bCanFirePistol = true;
	RevolverFireRate = 0.25;

	RootComponent = GetCapsuleComponent();

	GetArrowComponent()->SetupAttachment(RootComponent);

	GetMesh()->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Health <= 0) {
		Destroy();
	}
}

// Called to bind functionality to input
void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

