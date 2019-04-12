// Fill out your copyright notice in the Description page of Project Settings.

#include "MyCharacter.h"
#include "Revolver.h"
#include "Rifle.h"
#include "Projectile.h"
#include "MyProjectGameInstance.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/World.h"
#include "Engine/GameEngine.h"
#include "Math/UnrealMathUtility.h"
#include "Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Sound/SoundCue.h"
#include "Runtime/Engine/Public/TimerManager.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"


// Sets default values
AMyCharacter::AMyCharacter()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	KilledEnemies = 0;

	Health = 500.f;
	CharacterMovement->MaxWalkSpeed = 550.f;
	ZoomedCharSpeed = CharacterMovement->MaxWalkSpeed / 2.5f;
	NotZoomedCharSpeed = CharacterMovement->MaxWalkSpeed;
	SlowMoCapacity = 0.0f;

	PistolFireRate = 0.25f;
	RifleFireRate = 1.0f;

	LookSpeed = 1.5f;
	LookUpperLimit = -50.0f;
	LookLowerLimit = 65.0f;

	HealthRegenSpeed = 20.f;

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

	// Find the SlowMo cue in content browser by reference
	static ConstructorHelpers::FObjectFinder<USoundCue>
		CueAsset(TEXT("SoundCue'/Game/Assets/Sound/SlowMoCue.SlowMoCue'"));
	if (CueAsset.Succeeded() == true) {
		SlowMoWoosh = CueAsset.Object;
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("SlowMo Cue Not Found In MyChar!")));
	}
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Configure difficulty variables
	if (Cast<UMyProjectGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))->DifficultyAmount == Medium) {
		HealthRegenSpeed -= 3.75f;
	}
	else if (Cast<UMyProjectGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))->DifficultyAmount == Hard) {
		HealthRegenSpeed -= 7.5f;
		PistolMagazineLimit -= 10;
		CurrPistolMagazine -= 10;
		RifleMagazineLimit -= 3;
		CurrRifleMagazine -= 3;
	}
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
	if (!bCanFirePistol) {
		LerpPlayerToCamera(15.0f);
	}

	if (!bCanRifleAnim) {
		LerpPlayerToCamera(15.0f);
	}

	// Regenerate health
	if ((Health < 500.f) && !bZoomedKills) {
		Health += DeltaTime * HealthRegenSpeed;
	}

	// Slowly deplete SlowMo's capacity when ON
	if (bSlowMo && !bZoomedKills) {
		SlowMoCapacity -= DeltaTime * 1.5f;
		if (SlowMoCapacity <= 0.0f) {
			EnterSlowMo();		// Disable slow mo when out of capacity
		}
	}
	// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("%f"), LookSpeed));
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

// Sets input to 1.0 when clicking W
								// Sets input to -1.0 when clicking S
void AMyCharacter::MoveForward(float Input)
{
	SetForwardInput(Input);	

	if (Input != 0.0) {

		FVector SpringArmForward = SpringArm->GetForwardVector();

		AddMovementInput(SpringArmForward, Input);

		// Character turns to camera direction when moving
		LerpPlayerToCamera(8.0f);	
	}
}

void AMyCharacter::MoveRight(float Input)
{
	RightInput = Input;

	if (Input != 0.0) {

		FVector SpringArmRight = SpringArm->GetRightVector();

		AddMovementInput(SpringArmRight, Input);

		LerpPlayerToCamera(8.0f);
	}
}

// Sets input to 1.0 when moving mouse right
// Sets input to -1.0 when moving mouse left
void AMyCharacter::LookSide(float Input)
{
	// Camera rotates right when moving mouse right on X axis
	if (Input != 0.0) {		
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
		if (bSlowMo) {
			// Increase character speed when time is slowed for player's advantage when aiming
			CharacterMovement->MaxWalkSpeed = ZoomedCharSpeed * 2.5f;
		}
		else {
			CharacterMovement->MaxWalkSpeed = ZoomedCharSpeed;
		}
		SpringArm->TargetArmLength = FMath::FInterpTo(SpringArm->TargetArmLength, 150.0f, GetWorld()->GetDeltaSeconds(), 10.0f);
		SpringArm->SocketOffset = FMath::VInterpTo(SpringArm->SocketOffset, FVector(0.0f, 0.0f, 80.0f), GetWorld()->GetDeltaSeconds(), 10.0f);
		Camera->RelativeLocation = FMath::VInterpTo(Camera->RelativeLocation, FVector(0.0f, 65.0f, 0.0f), GetWorld()->GetDeltaSeconds(), 10.0f);

		bZooming = true;
	}
}

void AMyCharacter::CameraOutZoom()
{
	if (bSlowMo) {
		// Increase character speed when time is slowed for player's advantage when not aiming
		CharacterMovement->MaxWalkSpeed = NotZoomedCharSpeed * 2.5f;
	}
	else {
		CharacterMovement->MaxWalkSpeed = NotZoomedCharSpeed;
	}
	SpringArm->TargetArmLength = FMath::FInterpTo(SpringArm->TargetArmLength, 400.0f, GetWorld()->GetDeltaSeconds(), 10.0f);
	SpringArm->SocketOffset = FMath::VInterpTo(SpringArm->SocketOffset, FVector(0.0f, 0.0f, 150.0f), GetWorld()->GetDeltaSeconds(), 10.0f);
	Camera->RelativeLocation = FMath::VInterpTo(Camera->RelativeLocation, FVector(0.0f, 0.0f, 0.0f), GetWorld()->GetDeltaSeconds(), 10.0f);

	bZooming = false;
}

// Character's direction rotates to match camera's direction
void AMyCharacter::LerpPlayerToCamera(float Speed)
{
	float CurrYawRot = GetMesh()->GetRelativeTransform().GetRotation().Rotator().Yaw;
	float NewYawRot = SpringArm->GetRelativeTransform().GetRotation().Rotator().Yaw;
	FRotator CurrRot = FRotator(0.0f, CurrYawRot, 0.0f);
	FRotator NewRot = FRotator(0.0f, NewYawRot, 0.0f);

	GetMesh()->SetRelativeRotation(FMath::Lerp(CurrRot, NewRot, Speed * GetWorld()->GetDeltaSeconds()));
}

void AMyCharacter::Fire()
{
	if (WInHand == Pistol) {
		if (PistolActor && bCanFirePistol && (CurrPistolMagazine > 0)) {
			PistolActor->SpawnProjectile();
			bCanFirePistol = false;
			CurrPistolMagazine--;
			GetWorldTimerManager().SetTimer(PistolFireRateHandle, this, &AMyCharacter::ResetPistolFire, PistolFireRate, false, PistolFireRate);
		}
	}
	else if (WInHand == Rifle) {
		if (RifleActor && bCanFireRifle && (CurrRifleMagazine > 0)) {
			RifleActor->SpawnProjectile();
			// Use this variable to speed up animation(visual purposes)
			bCanRifleAnim = false;		
			bCanFireRifle = false;
			CurrRifleMagazine--;
			GetWorldTimerManager().SetTimer(RifleAnimHandle, this, &AMyCharacter::ResetRifleAnim, 0.4f, false, 0.4f);
			GetWorldTimerManager().SetTimer(RifleFireRateHandle, this, &AMyCharacter::ResetRifleFire, RifleFireRate, false, RifleFireRate);
		}
	}
}

void AMyCharacter::ChangeToPistol()
{
	// Can't change weapons when aimed
	if (bHavePistol && (WInHand != Pistol) && !bZooming) {		

		if (WInHand == None) {
			GetPistolActor()->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("HandSocketPistol"));
		}
		if (WInHand == Rifle) {
			GetPistolActor()->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("HandSocketPistol"));
			GetRifleActor()->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("RifleSocket"));
		}
		WInHand = Pistol;
	}
}

void AMyCharacter::ChangeToRifle()
{
	// Can't change weapons when aimed
	if (bHaveRifle && (WInHand != Rifle) && !bZooming) {		

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
	if (!bSlowMo && (SlowMoCapacity > 0.0f)) {
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.4f);
		CharacterMovement->MaxWalkSpeed *= 2.5f;
		PistolFireRate /= 2.5f;
		RifleFireRate /= 2.5f;
		HealthRegenSpeed *= 2.5f;
		bSlowMo = true;

		float VolumeControl = Cast<UMyProjectGameInstance>(GetWorld()->GetGameInstance())->VolumeControl;
		UGameplayStatics::PlaySound2D(GetWorld(), SlowMoWoosh, VolumeControl, SlowMoWoosh->GetPitchMultiplier());
	}
	else if (bSlowMo) {
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
		CharacterMovement->MaxWalkSpeed /= 2.5f;
		PistolFireRate *= 2.5f;
		RifleFireRate *= 2.5f;
		HealthRegenSpeed /= 2.5f;
		bSlowMo = false;
	}
}

void AMyCharacter::ZoomedKills(float Distance)
{
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.25f);
	bZoomedKills = true;

	// Hide the character and weapons
	SetActorHiddenInGame(true);
	if (PistolActor) {
		PistolActor->SetActorHiddenInGame(true);
	}
	if (RifleActor) {
		RifleActor->SetActorHiddenInGame(true);
	}
	// Generate new FOV based on enemy's distance from player
	float NewFOV = 30000 / (Distance / 1.1f);

	DisableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	GetCamera()->SetFieldOfView(NewFOV);
	GetWorldTimerManager().SetTimer(ZoomedKillHandle, this, &AMyCharacter::ResetZoomedKills, 0.5f, false, 0.5f);
}

void AMyCharacter::ResetZoomedKills()
{
	GetWorldTimerManager().ClearTimer(ZoomedKillHandle);
	bZoomedKills = false;

	// Check if SlowMo was used
	if (bSlowMo) {
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.4f);
	}
	else {
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.f);
	}

	// Show the character and weapons
	SetActorHiddenInGame(false);
	if (PistolActor) {
		PistolActor->SetActorHiddenInGame(false);
	}
	if (RifleActor) {
		RifleActor->SetActorHiddenInGame(false);
	}

	GetCamera()->SetFieldOfView(90.f);
	EnableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));
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

// Calls DestroyChar after 2 seconds
void AMyCharacter::DestroyAfterTime()
{
	GetWorldTimerManager().SetTimer(DestroyHandle, this, &AMyCharacter::DestroyChar, 2, false, 2);
}

// Opens menu level and deallocates memory
void AMyCharacter::DestroyChar()
{
	for (FEnemyHandler* Struct : BankEnemyHandlerArray)
	{
		delete Struct;
	}
	for (FEnemyHandler* Struct : HotelEnemyHandlerArray)
	{
		delete Struct;
	}
	for (FEnemyHandler* Struct : StoreEnemyHandlerArray)
	{
		delete Struct;
	}
	for (FEnemyHandler* Struct : SaloonEnemyHandlerArray)
	{
		delete Struct;
	}
	for (FEnemyHandler* Struct : Saloon2EnemyHandlerArray)
	{
		delete Struct;
	}

	UGameplayStatics::OpenLevel(GetWorld(), TEXT("MenuLevel"));
}