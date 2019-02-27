// Fill out your copyright notice in the Description page of Project Settings.

#include "MyCharacter.h"
#include "Revolver.h"
#include "Rifle.h"
#include "Projectile.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Engine/World.h"
#include "Engine/GameEngine.h"
#include "Math/UnrealMathUtility.h"
#include "Kismet/GameplayStatics.h"
#include "Runtime/Engine/Public/TimerManager.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"


// Sets default values
AMyCharacter::AMyCharacter()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Health = 500.f;
	CharacterSpeed = 450.0f;
	ZoomedCharSpeed = CharacterSpeed / 2.5f;
	NotZoomedCharSpeed = CharacterSpeed;
	SlowMoCapacity = 0.0f;

	PistolFireRate = 0.25f;
	RifleFireRate = 1.0f;

	LookSpeed = 1.5f;
	LookUpperLimit = -50.0f;
	LookLowerLimit = 65.0f;

	HealthRegenSpeed = 12.5f;

	bZooming = false;

	bSlowMo = false;

	WInHand = None;

	PistolMagazineLimit = 30;
	CurrPistolMagazine = 30;
	RifleMagazineLimit = 10;
	CurrRifleMagazine = 10;
	
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(GetCapsuleComponent());

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

	// Call CameraZoom() in tick so it changes position smoothly
	if (bZooming == true && SpringArm->TargetArmLength != 151.0f) {
		CameraZoom();	
		LerpPlayerToCamera(6.0f);
	}

	// Call CameraOutZoom() in tick so it changes position smoothly
	if (bZooming == false && SpringArm->TargetArmLength <= 399.0f) {
		CameraOutZoom();
	}

	// Character to face camera's direction when shooting
	if (GetCanFirePistol() == false) {
		LerpPlayerToCamera(15.0f);
	}

	if (GetCanRifleAnim() == false) {
		LerpPlayerToCamera(15.0f);
	}

	// Regenerate health
	if (Health < 500.f) {
		Health += DeltaTime * HealthRegenSpeed;
	}

	// Slowly deplete SlowMo's capacity when ON
	if (bSlowMo == true) {
		SlowMoCapacity -= DeltaTime * 1.5f;
		if (SlowMoCapacity <= 0.0f) {
			EnterSlowMo();		// Disable slow mo when out of capacity
		}
	}

	// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("%s"), *SpringArm->GetForwardVector().ToString()));
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
	PlayerInputComponent->BindAction("SlowMo", IE_Pressed, this, &AMyCharacter::EnterSlowMo);
}

void AMyCharacter::MoveForward(float Input)
{
	SetForwardInput(Input);		// Sets input to 1.0 when clicking W
								// Sets input to -1.0 when clicking S
	if (Input != 0.0) {
		FVector CurrLoc = GetActorLocation();
		FVector NewLoc;
		FVector SpringArmForward = SpringArm->GetForwardVector();

		if (RightInput != 0.0) {	// Lowers the speed when holding both W/S and D/A so that character doesn't move faster diagonally
			NewLoc = CurrLoc + (SpringArmForward * (CharacterSpeed / 1.3) * Input * GetWorld()->GetDeltaSeconds());
		}
		else {
			NewLoc = CurrLoc + (SpringArmForward * CharacterSpeed * Input * GetWorld()->GetDeltaSeconds());
		}
		SetActorLocation(NewLoc);
		
		LerpPlayerToCamera(6.0f);	// Character turns to camera direction when moving
	}
}

void AMyCharacter::MoveRight(float Input)
{
	RightInput = Input;

	if (Input != 0.0) {
		FVector CurrLoc = GetActorLocation();
		FVector NewLoc;
		FVector SpringArmRight = SpringArm->GetRightVector();

		if (GetForwardInput() != 0.0) {
			NewLoc = CurrLoc + (SpringArmRight * (CharacterSpeed / 1.3) * Input * GetWorld()->GetDeltaSeconds());
		}
		else {
			NewLoc = CurrLoc + (SpringArmRight * CharacterSpeed * Input * GetWorld()->GetDeltaSeconds());
		}
		SetActorLocation(NewLoc);

		LerpPlayerToCamera(6.0f);
	}
}

// Sets input to 1.0 when moving mouse right; Sets input to -1.0 when moving mouse left
void AMyCharacter::LookSide(float Input)
{
	if (Input != 0.0) {		// Camera rotates right when moving mouse right on X axis
		float NewRot = LookSpeed * Input;
		SpringArm->AddRelativeRotation(FRotator(0.0f, NewRot, 0.0f));
	}
}

void AMyCharacter::LookUp(float Input)
{
	if (Input != 0.0f) {	// Camera rotates up when moving mouse up on Y axis
		
		float CurrRot = SpringArm->GetRelativeTransform().GetRotation().Rotator().Pitch;
		float NewRot = LookSpeed * Input;

		// Limit the rotation at some point
		if ((CurrRot + NewRot) > LookUpperLimit && (CurrRot + NewRot) < LookLowerLimit) {	
			SpringArm->AddRelativeRotation(FRotator(NewRot, 0.0f, 0.0f));
		}
	}
}

void AMyCharacter::CameraZoom()
{
	if (WInHand != None) {
		if (bSlowMo == true) {
			CharacterSpeed = ZoomedCharSpeed * 2.5;		// Increase character speed when time is slowed for player's advantage when aiming
		}
		else {
			CharacterSpeed = ZoomedCharSpeed;
		}
		SpringArm->TargetArmLength = FMath::FInterpTo(SpringArm->TargetArmLength, 150.0f, GetWorld()->GetDeltaSeconds(), 10.0f);
		SpringArm->SocketOffset = FMath::VInterpTo(SpringArm->SocketOffset, FVector(0.0f, 0.0f, 80.0f), GetWorld()->GetDeltaSeconds(), 10.0f);
		Camera->RelativeLocation = FMath::VInterpTo(Camera->RelativeLocation, FVector(0.0f, 65.0f, 0.0f), GetWorld()->GetDeltaSeconds(), 10.0f);

		bZooming = true;
	}
}

void AMyCharacter::CameraOutZoom()
{
	if (bSlowMo == true) {
		CharacterSpeed = NotZoomedCharSpeed * 2.5;		// Increase character speed when time is slowed for player's advantage when not aiming
	}
	else {
		CharacterSpeed = NotZoomedCharSpeed;
	}
	SpringArm->TargetArmLength = FMath::FInterpTo(SpringArm->TargetArmLength, 400.0f, GetWorld()->GetDeltaSeconds(), 10.0f);
	SpringArm->SocketOffset = FMath::VInterpTo(SpringArm->SocketOffset, FVector(0.0f, 0.0f, 150.0f), GetWorld()->GetDeltaSeconds(), 10.0f);
	Camera->RelativeLocation = FMath::VInterpTo(Camera->RelativeLocation, FVector(0.0f, 0.0f, 0.0f), GetWorld()->GetDeltaSeconds(), 10.0f);

	bZooming = false;
}

// Character's direction rotates to match camera's direction
void AMyCharacter::LerpPlayerToCamera(float Speed)
{
	float CurrRot = GetMesh()->GetRelativeTransform().GetRotation().Rotator().Yaw;
	float NewRot = SpringArm->GetRelativeTransform().GetRotation().Rotator().Yaw;
	GetMesh()->SetRelativeRotation(FMath::Lerp(FRotator(0.0f, CurrRot, 0.0f), FRotator(0.0f, NewRot, 0.0f), Speed * GetWorld()->GetDeltaSeconds()));
}

void AMyCharacter::Fire()
{
	if (WInHand == Pistol) {
		if ((PistolActor != nullptr) && (bCanFirePistol == true) && (CurrPistolMagazine > 0)) {
			PistolActor->SpawnProjectile();
			bCanFirePistol = false;
			CurrPistolMagazine--;
			GetWorldTimerManager().SetTimer(PistolFireRateHandle, this, &AMyCharacter::ResetPistolFire, PistolFireRate, false, PistolFireRate);
		}
	}
	if (WInHand == Rifle) {
		if ((RifleActor != nullptr) && (bCanFireRifle == true) && (CurrRifleMagazine > 0)) {
			RifleActor->SpawnProjectile();
			bCanRifleAnim = false;		// Use this variable to speed up animation(visual purposes)
			bCanFireRifle = false;
			CurrRifleMagazine--;
			GetWorldTimerManager().SetTimer(RifleAnimHandle, this, &AMyCharacter::ResetRifleAnim, 0.4f, false, 0.4f);
			GetWorldTimerManager().SetTimer(RifleFireRateHandle, this, &AMyCharacter::ResetRifleFire, RifleFireRate, false, RifleFireRate);
		}
	}
}

void AMyCharacter::ChangeToPistol()
{
	if ((bHavePistol == true) && (WInHand != Pistol) && (bZooming == false)) {		// Can't change weapons when aimed

		if (WInHand == None) {
			GetPistolActor()->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("HandSocketPistol"));
		}
		if (WInHand == Rifle){
			GetPistolActor()->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("HandSocketPistol"));
			GetRifleActor()->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("RifleSocket"));
		}
		WInHand = Pistol;
	}
}

void AMyCharacter::ChangeToRifle()
{
	if ((bHaveRifle == true) && (WInHand != Rifle) && (bZooming == false)) {		// Can't change weapons when aimed

		if (WInHand == None) {
			GetRifleActor()->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("HandSocketRifle"));
		}
		if (WInHand == Pistol) {
			GetRifleActor()->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("HandSocketRifle"));
			GetPistolActor()->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("PistolSocket"));
		}
		WInHand = Rifle;
	}
}

void AMyCharacter::EnterSlowMo()
{
	if ((bSlowMo == false) && (SlowMoCapacity > 0.0f)) {
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.4f);
		CharacterSpeed *= 2.5f;
		PistolFireRate /= 2.5f;
		RifleFireRate /= 2.5f;
		HealthRegenSpeed *= 2.5f;
		bSlowMo = true;
	}
	else if(bSlowMo == true) {
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
		CharacterSpeed /= 2.5f;
		PistolFireRate *= 2.5f;
		RifleFireRate *= 2.5f;
		HealthRegenSpeed /= 2.5f;
		bSlowMo = false;
	}
}

void AMyCharacter::ResetPistolFire()
{
	GetWorldTimerManager().ClearTimer(PistolFireRateHandle);
	bCanFirePistol = true;
}

void AMyCharacter::ResetRifleFire()
{
	GetWorldTimerManager().ClearTimer(RifleFireRateHandle);
	bCanFireRifle = true;
}

void AMyCharacter::ResetRifleAnim()
{
	GetWorldTimerManager().ClearTimer(RifleAnimHandle);
	bCanRifleAnim = true;
}