// Fill out your copyright notice in the Description page of Project Settings.

#include "MyCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Engine/World.h"
#include "Engine/GameEngine.h"
#include "Math/UnrealMathUtility.h"
#include "Runtime/Engine/Public/TimerManager.h"


// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ForwardInput = 0.0f;
	RightInput = 0.0f;

	bCanZoom = false;
	bCanOutZoom = false;

	PlayerSpeed = 400.0f;
	LookSpeed = 150.0f;
	LookUpperLimit = -50.0f;
	LookLowerLimit = 65.0f;

	CapsuleCollision = FindComponentByClass<UCapsuleComponent>();
	RootComponent = CapsuleCollision;

	PlayerMesh = FindComponentByClass<USkeletalMeshComponent>();
	PlayerMesh->SetupAttachment(CapsuleCollision);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(CapsuleCollision);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bCanZoom == true && SpringArm->TargetArmLength != 151.0f) {
		CameraZoom();
		LerpPlayerToRot();
	}

	if (bCanOutZoom == true && SpringArm->TargetArmLength <= 399.0f) {
		CameraOutZoom();
	}
}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMyCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMyCharacter::MoveRight);
	PlayerInputComponent->BindAxis("MouseX", this, &AMyCharacter::LookSide);
	PlayerInputComponent->BindAxis("MouseY", this, &AMyCharacter::LookUp);
	PlayerInputComponent->BindAction("CameraZoom", IE_Pressed, this, &AMyCharacter::CameraZoom);
	PlayerInputComponent->BindAction("CameraZoom", IE_Released, this, &AMyCharacter::CameraOutZoom);
}

void AMyCharacter::MoveForward(float input)
{
	ForwardInput = input;

	if (input != 0.0) {
		FVector CurrLoc = GetActorLocation();
		FVector SpringArmForward = SpringArm->GetForwardVector();
		FVector NewLoc = CurrLoc + (SpringArmForward * PlayerSpeed * input * GetWorld()->GetDeltaSeconds());
		SetActorLocation(NewLoc);
		
		LerpPlayerToRot();
	}
}

void AMyCharacter::MoveRight(float input)
{
	RightInput = input;

	if (input != 0.0) {
		FVector CurrLoc = GetActorLocation();
		FVector NewLoc = CurrLoc + (SpringArm->GetRightVector() * PlayerSpeed * input * GetWorld()->GetDeltaSeconds());
		SetActorLocation(NewLoc);

		LerpPlayerToRot();
	}
}

void AMyCharacter::LookSide(float input)
{
	if (input != 0.0) {
		float NewRot = LookSpeed * input * GetWorld()->GetDeltaSeconds();
		SpringArm->AddRelativeRotation(FRotator(0.0f, NewRot, 0.0f));
	}
}

void AMyCharacter::LookUp(float input)
{
	if (input != 0.0f) {
		
		float CurrRot = SpringArm->GetRelativeTransform().GetRotation().Rotator().Pitch;
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("%f"), CurrRot));

		float NewRot = LookSpeed * input * GetWorld()->GetDeltaSeconds();
		if ((CurrRot + NewRot) > -50.0f && (CurrRot + NewRot) < 65.0f) {
			SpringArm->AddRelativeRotation(FRotator(NewRot, 0.0f, 0.0f));
		}
	}
}

void AMyCharacter::CameraZoom()
{
	bCanZoom = true;
	bCanOutZoom = false;

	PlayerSpeed = 150.0f;

	SpringArm->TargetArmLength = FMath::FInterpTo(SpringArm->TargetArmLength, 150.0f, GetWorld()->GetDeltaSeconds(), 10.0f);
	SpringArm->SocketOffset = FMath::VInterpTo(SpringArm->SocketOffset, FVector(0.0f, 0.0f, 80.0f), GetWorld()->GetDeltaSeconds(), 10.0f);
	Camera->RelativeLocation = FMath::VInterpTo(Camera->RelativeLocation, FVector(0.0f, 65.0f, 0.0f), GetWorld()->GetDeltaSeconds(), 10.0f);
}

void AMyCharacter::CameraOutZoom()
{
	bCanOutZoom = true;
	bCanZoom = false;

	PlayerSpeed = 400.0f;

	SpringArm->TargetArmLength = FMath::FInterpTo(SpringArm->TargetArmLength, 400.0f, GetWorld()->GetDeltaSeconds(), 10.0f);
	SpringArm->SocketOffset = FMath::VInterpTo(SpringArm->SocketOffset, FVector(0.0f, 0.0f, 150.0f), GetWorld()->GetDeltaSeconds(), 10.0f);
	Camera->RelativeLocation = FMath::VInterpTo(Camera->RelativeLocation, FVector(0.0f, 0.0f, 0.0f), GetWorld()->GetDeltaSeconds(), 10.0f);
}



void AMyCharacter::LerpPlayerToRot()
{
	float CurrRot = PlayerMesh->GetRelativeTransform().GetRotation().Rotator().Yaw;
	float NewRot = SpringArm->GetRelativeTransform().GetRotation().Rotator().Yaw;
	PlayerMesh->SetRelativeRotation(FMath::Lerp(FRotator(0.0f, CurrRot, 0.0f), FRotator(0.0f, NewRot, 0.0f), 6.0f * GetWorld()->GetDeltaSeconds()));
}


