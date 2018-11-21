// Fill out your copyright notice in the Description page of Project Settings.

#include "MyCharacter.h"
#include "Revolver.h"
#include "Projectile.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Engine/World.h"
#include "Engine/GameEngine.h"
#include "Math/UnrealMathUtility.h"
#include "Runtime/Engine/Public/TimerManager.h"


// Sets default values
AMyCharacter::AMyCharacter()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Health = 100;
	CharacterSpeed = 400.0f;

	LookSpeed = 150.0f;
	LookUpperLimit = -50.0f;
	LookLowerLimit = 65.0f;

	bZooming = false;
	bOutZooming = false;

	WInHand = None;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(GetCapsuleComponent());

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	CharacterNormalSpeed = CharacterSpeed;
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	if (bZooming == true && SpringArm->TargetArmLength != 151.0f) {
		CameraZoom();
		LerpPlayerToCamera(6.0f);
	}

	if (bOutZooming == true && SpringArm->TargetArmLength <= 399.0f) {
		CameraOutZoom();
	}


	if (bCanFirePistol == false) {
		LerpPlayerToCamera(15.0f);
	}
	
	
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("%d"), WInHand));
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
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AMyCharacter::Fire);
	PlayerInputComponent->BindAction("ChangeToPistol", IE_Pressed, this, &AMyCharacter::ChangeToPistol);
	PlayerInputComponent->BindAction("ChangeToRifle", IE_Pressed, this, &AMyCharacter::ChangeToRifle);
}

void AMyCharacter::MoveForward(float Input)
{
	ForwardInput = Input;

	if (Input != 0.0) {
		FVector CurrLoc = GetActorLocation();
		FVector NewLoc;
		FVector SpringArmForward = SpringArm->GetForwardVector();
		if (RightInput != 0.0) {
			NewLoc = CurrLoc + (SpringArmForward * (CharacterSpeed / 1.3) * Input * GetWorld()->GetDeltaSeconds());
		}
		else {
			NewLoc = CurrLoc + (SpringArmForward * CharacterSpeed * Input * GetWorld()->GetDeltaSeconds());
		}
		SetActorLocation(NewLoc);
		
		LerpPlayerToCamera(6.0f);
	}
}

void AMyCharacter::MoveRight(float Input)
{
	RightInput = Input;

	if (Input != 0.0) {
		FVector CurrLoc = GetActorLocation();
		FVector NewLoc;
		if (ForwardInput != 0.0) {
			NewLoc = CurrLoc + (SpringArm->GetRightVector() * (CharacterSpeed / 1.3) * Input * GetWorld()->GetDeltaSeconds());
		}
		else {
			NewLoc = CurrLoc + (SpringArm->GetRightVector() * CharacterSpeed * Input * GetWorld()->GetDeltaSeconds());
		}
		SetActorLocation(NewLoc);

		LerpPlayerToCamera(6.0f);
	}
}

void AMyCharacter::LookSide(float Input)
{
	if (Input != 0.0) {
		float NewRot = LookSpeed * Input * GetWorld()->GetDeltaSeconds();
		SpringArm->AddRelativeRotation(FRotator(0.0f, NewRot, 0.0f));
	}
}

void AMyCharacter::LookUp(float Input)
{
	if (Input != 0.0f) {
		
		float CurrRot = SpringArm->GetRelativeTransform().GetRotation().Rotator().Pitch;

		float NewRot = LookSpeed * Input * GetWorld()->GetDeltaSeconds();
		if ((CurrRot + NewRot) > -50.0f && (CurrRot + NewRot) < 65.0f) {
			SpringArm->AddRelativeRotation(FRotator(NewRot, 0.0f, 0.0f));
		}
	}
}

void AMyCharacter::CameraZoom()
{
	if (WInHand != None) {
		bZooming = true;
		bOutZooming = false;

		CharacterSpeed = CharacterNormalSpeed / 2.6f;

		SpringArm->TargetArmLength = FMath::FInterpTo(SpringArm->TargetArmLength, 150.0f, GetWorld()->GetDeltaSeconds(), 10.0f);
		SpringArm->SocketOffset = FMath::VInterpTo(SpringArm->SocketOffset, FVector(0.0f, 0.0f, 80.0f), GetWorld()->GetDeltaSeconds(), 10.0f);
		Camera->RelativeLocation = FMath::VInterpTo(Camera->RelativeLocation, FVector(0.0f, 65.0f, 0.0f), GetWorld()->GetDeltaSeconds(), 10.0f);
	}
}

void AMyCharacter::CameraOutZoom()
{
	bOutZooming = true;
	bZooming = false;

	CharacterSpeed = CharacterNormalSpeed;

	SpringArm->TargetArmLength = FMath::FInterpTo(SpringArm->TargetArmLength, 400.0f, GetWorld()->GetDeltaSeconds(), 10.0f);
	SpringArm->SocketOffset = FMath::VInterpTo(SpringArm->SocketOffset, FVector(0.0f, 0.0f, 150.0f), GetWorld()->GetDeltaSeconds(), 10.0f);
	Camera->RelativeLocation = FMath::VInterpTo(Camera->RelativeLocation, FVector(0.0f, 0.0f, 0.0f), GetWorld()->GetDeltaSeconds(), 10.0f);
}

void AMyCharacter::LerpPlayerToCamera(float Speed)
{
	float CurrRot = GetMesh()->GetRelativeTransform().GetRotation().Rotator().Yaw;
	float NewRot = SpringArm->GetRelativeTransform().GetRotation().Rotator().Yaw;
	GetMesh()->SetRelativeRotation(FMath::Lerp(FRotator(0.0f, CurrRot, 0.0f), FRotator(0.0f, NewRot, 0.0f), Speed * GetWorld()->GetDeltaSeconds()));
}

void AMyCharacter::Fire()
{
	if (WInHand == Pistol) {
		if (RevolverRef != NULL && bCanFirePistol == true) {
			if (bZooming == false) {
				GetWorldTimerManager().SetTimer(NoZoomFire, this, &AMyCharacter::FireAfterDelay, 0.25f, false, 0.25f);

			}
			else {
				RevolverRef->SpawnProjectile();
			}
			bCanFirePistol = false;
			GetWorldTimerManager().SetTimer(PistolFireRate, this, &AMyCharacter::ResetPistolFire, RevolverFireRate, false, RevolverFireRate);
		}
	}
	if (WInHand == Rifle) {

	}
}

void AMyCharacter::ChangeToPistol()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Entering")));
	if (bHavePistol == true && WInHand != Pistol) {
		RevolverRef->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("WeaponSocket"));
		WInHand = Pistol;
	}
}

void AMyCharacter::ChangeToRifle()
{
	if (bHaveRifle == true && WInHand != Rifle) {

	}
}

void AMyCharacter::ResetPistolFire()
{
	GetWorldTimerManager().ClearTimer(PistolFireRate);
	bCanFirePistol = true;
}

void AMyCharacter::FireAfterDelay()
{
	GetWorldTimerManager().ClearTimer(NoZoomFire);
	RevolverRef->SpawnProjectile();
}



