// Fill out your copyright notice in the Description page of Project Settings.

#include "Rifle.h"
#include "Projectile.h"
#include "MyCharacter.h"
#include "CharacterBase.h"
#include "WindowEnemy.h"
#include "MyProjectGameInstance.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Camera/CameraComponent.h"
#include "Engine/GameEngine.h"
#include "Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Public/TimerManager.h"
#include "Runtime/Engine/Classes/Sound/SoundCue.h"
#include "EngineUtils.h"


ARifle::ARifle() {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Damage = 50;
	ProjectileOffsetNoZoom = 2.5f;
}

void ARifle::BeginPlay()
{
	Super::BeginPlay();
}

void ARifle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARifle::SpawnProjectile()
{
	FRotator SpawnRotation;
	FVector SpawnLocation = GunMesh->GetSocketLocation("Muzzle");

	if (Cast<AMyCharacter>(GetCharacterActor()) != NULL) {

		AMyCharacter* MainCharacter = Cast<AMyCharacter>(CharacterActor);

		int32 ChanceToHit = FMath::FRandRange(1, 100);

		if (MainCharacter->GetZooming() == true) {

			SpawnRotation = GetHitRot(SpawnLocation, MainCharacter);
		}
		else {

			if (ChanceToHit <= 35) {		// There's a 35% chance the bullet will go exactly at crosshair when not zooming
				SpawnRotation = GetHitRot(SpawnLocation, MainCharacter);
			}
			else {
				float BulletOffsetPitch;
				float BulletOffsetYaw;
				BulletOffsetPitch = FMath::RandRange(-ProjectileOffsetNoZoom, ProjectileOffsetNoZoom);		// Add a random deviation to the crosshair's location
				BulletOffsetYaw = FMath::RandRange(-ProjectileOffsetNoZoom, ProjectileOffsetNoZoom);
				FRotator CurrRot = GetHitRot(SpawnLocation, MainCharacter);
				SpawnRotation = FRotator(CurrRot.Pitch + BulletOffsetPitch, CurrRot.Yaw + BulletOffsetYaw, CurrRot.Roll);
			}
		}
	}
	else if (Cast<AWindowEnemy>(CharacterActor) != NULL) {

		AWindowEnemy* EnemyCharacter = Cast<AWindowEnemy>(CharacterActor);
		// Find the rotation to look at main character
		SpawnRotation = UKismetMathLibrary::FindLookAtRotation(SpawnLocation, EnemyCharacter->GetMainCharacterActor()->GetActorLocation());

		int32 ChanceToHit = FMath::FRandRange(1, 100);
		int32 Percentage = 35;

		// Configure difficulty variables
		if (Cast<UMyProjectGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))->DifficultyAmount == Medium) {
			Percentage += 25;
		}
		else if (Cast<UMyProjectGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))->DifficultyAmount == Hard) {
			Percentage += 50;
		}
		// There's a Percentage chance the bullet will go exactly at player's character
		if (ChanceToHit <= Percentage) {
			float BulletOffsetPitch;
			float BulletOffsetYaw;
			BulletOffsetPitch = FMath::RandRange(-ProjectileOffsetNoZoom * 2.5f, ProjectileOffsetNoZoom * 2.5f);
			BulletOffsetYaw = FMath::RandRange(-ProjectileOffsetNoZoom * 2.5f, ProjectileOffsetNoZoom * 2.5f);
			SpawnRotation = FRotator(SpawnRotation.Pitch + BulletOffsetPitch, SpawnRotation.Yaw + BulletOffsetYaw, SpawnRotation.Roll);
		}
	}
	// Spawn the projectile and set it's shooter and damage
	FActorSpawnParameters ActorSpawnParams;
	AProjectile* SpawnedProjectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);

	SpawnedProjectile->SetCharacterActor(CharacterActor);
	SpawnedProjectile->SetDamage(Damage);
	
	// Wait 0.2 seconds when player is firing for animation synchronization
	if (Cast<AMyCharacter>(CharacterActor) != NULL) {
		AMyCharacter* MainChar = Cast<AMyCharacter>(CharacterActor);
		if (MainChar->GetZooming() == true) {
			SpawnEmitter();
		}
		else {
			if (MainChar->GetSlowMo() == true) {
				GetWorldTimerManager().SetTimer(ParticleDelayHandle, this, &ARifle::SpawnEmitter, 0.08f, false, 0.08f);
			}
			else {
				GetWorldTimerManager().SetTimer(ParticleDelayHandle, this, &ARifle::SpawnEmitter, 0.2f, false, 0.2f);
			}
		}
	}
	else {
		SpawnEmitter();
	}
}

void ARifle::SpawnEmitter()
{
	UGameplayStatics::SpawnEmitterAtLocation(this, FireExplosion, GunMesh->GetSocketLocation("Muzzle"), GetActorRotation(), FVector(0.1f, 0.1f, 0.1f));
	if (Cast<AMyCharacter>(CharacterActor) != NULL) {

		float VolumeControl = Cast<UMyProjectGameInstance>(GetWorld()->GetGameInstance())->VolumeControl;
		UGameplayStatics::PlaySound2D(GetWorld(), RifleShot, VolumeControl, RifleShot->GetPitchMultiplier());
	}
	else {
		float VolumeControl = Cast<UMyProjectGameInstance>(GetWorld()->GetGameInstance())->VolumeControl;
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), RifleShot3D, GunMesh->GetSocketLocation("Muzzle"), VolumeControl, RifleShot3D->GetPitchMultiplier());
	}
}

void ARifle::OnEnterSphere(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<ACharacterBase>(OtherActor) != NULL) {

		ACharacterBase* CharacterEntered = Cast<ACharacterBase>(OtherActor);

		if ((CharacterEntered->GetHaveRifle() == false) && (CharacterActor == nullptr)) {
			CharacterEntered->SetRifleActor(this);
			CharacterEntered->SetHaveRifle(true);
			CharacterActor = CharacterEntered;
			this->AttachToComponent(CharacterEntered->GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("RifleSocket"));
			SphereCollision->SetSimulatePhysics(false);		// Enable it's physics when character dies

			if (Cast<AMyCharacter>(CharacterEntered) != NULL) {

				float VolumeControl = Cast<UMyProjectGameInstance>(GetWorld()->GetGameInstance())->VolumeControl;
				UGameplayStatics::PlaySound2D(GetWorld(), PickUp, VolumeControl, PickUp->GetPitchMultiplier());
			}
		}
		else {
			if ((Cast<AMyCharacter>(CharacterEntered) != NULL) && (CharacterActor == nullptr)) {
				AMyCharacter* MainChar = Cast<AMyCharacter>(CharacterEntered);
				if (MainChar->GetCurrRifleMagazine() < MainChar->GetRifleMagazineLimit()) {
					MainChar->SetCurrRifleMagazine(MainChar->GetRifleMagazineLimit());
					Destroy();

					float VolumeControl = Cast<UMyProjectGameInstance>(GetWorld()->GetGameInstance())->VolumeControl;
					UGameplayStatics::PlaySound2D(GetWorld(), PickUp, VolumeControl, PickUp->GetPitchMultiplier());
				}
			}
		}
	}
}
