// Fill out your copyright notice in the Description page of Project Settings.

#include "Revolver.h"
#include "Projectile.h"
#include "MyCharacter.h"
#include "CharacterBase.h"
#include "GroundEnemy.h"
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


// Sets default values
ARevolver::ARevolver()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Damage = 25;
	ProjectileOffsetNoZoom = 2.5f;
	ProjectileOffsetZoom = 1.5f;
}

// Called when the game starts or when spawned
void ARevolver::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARevolver::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARevolver::SpawnProjectile()
{
	FRotator SpawnRotation;
	FVector SpawnLocation = GunMesh->GetSocketLocation("Muzzle");

	if (Cast<AMyCharacter>(CharacterActor) != NULL) {

		AMyCharacter* MainCharacter = Cast<AMyCharacter>(CharacterActor);

		int32 ChanceToHit = FMath::FRandRange(1, 100);

		if (MainCharacter->GetZooming() == true) {
			// There's a 30% chance the bullet will go exactly at crosshair when zooming
			if (ChanceToHit <= 30) {		
				SpawnRotation = GetHitRot(SpawnLocation, MainCharacter);
			}
			else {
				float BulletOffsetPitch;
				float BulletOffsetYaw;
				// Add a random deviation to the crosshair's location
				BulletOffsetPitch = FMath::RandRange(-ProjectileOffsetZoom, ProjectileOffsetZoom);		
				BulletOffsetYaw = FMath::RandRange(-ProjectileOffsetZoom, ProjectileOffsetZoom);
				FRotator CurrRot = GetHitRot(SpawnLocation, MainCharacter);
				SpawnRotation = FRotator(CurrRot.Pitch + BulletOffsetPitch, CurrRot.Yaw + BulletOffsetYaw, CurrRot.Roll);	
			}
		}
		else {
			// There's a 15% chance the bullet will go exactly at crosshair when not zooming
			if (ChanceToHit <= 15) {		
				SpawnRotation = GetHitRot(SpawnLocation, MainCharacter);
			}
			else {
				float BulletOffsetPitch;
				float BulletOffsetYaw;
				// Add an even bigger random deviation to the crosshair's location
				BulletOffsetPitch = FMath::RandRange(-ProjectileOffsetNoZoom, ProjectileOffsetNoZoom);		
				BulletOffsetYaw = FMath::RandRange(-ProjectileOffsetNoZoom, ProjectileOffsetNoZoom);
				FRotator CurrRot = GetHitRot(SpawnLocation, MainCharacter);
				SpawnRotation = FRotator(CurrRot.Pitch + BulletOffsetPitch, CurrRot.Yaw + BulletOffsetYaw, CurrRot.Roll);
			}
		}
	}
	else if (Cast<AGroundEnemy>(GetCharacterActor()) != NULL) {

		AGroundEnemy* EnemyCharacter = Cast<AGroundEnemy>(CharacterActor);
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
		if (ChanceToHit <= Percentage) {		
			float BulletOffsetPitch;
			float BulletOffsetYaw;
			BulletOffsetPitch = FMath::RandRange(-ProjectileOffsetZoom, ProjectileOffsetZoom);
			BulletOffsetYaw = FMath::RandRange(-ProjectileOffsetZoom, ProjectileOffsetZoom);
			SpawnRotation = FRotator(SpawnRotation.Pitch + BulletOffsetPitch, SpawnRotation.Yaw + BulletOffsetYaw, SpawnRotation.Roll);
		}
	}
	// Spawn the projectile and set it's shooter and damage
	FActorSpawnParameters ActorSpawnParams;
	AProjectile* SpawnedProjectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);

	SpawnedProjectile->SetCharacterActor(GetCharacterActor());
	SpawnedProjectile->SetDamage(Damage);

	// Wait 0.2 seconds when player is firing for animation synchronization
	if (Cast<AMyCharacter>(CharacterActor) != NULL) {
		AMyCharacter* MainChar = Cast<AMyCharacter>(CharacterActor);
		if (MainChar->GetZooming() == true) {
			SpawnEmitter();
		}
		else {
			if (MainChar->GetSlowMo() == true) {
				GetWorldTimerManager().SetTimer(ParticleDelayHandle, this, &ARevolver::SpawnEmitter, 0.08f, false, 0.08f);
			}
			else {
				GetWorldTimerManager().SetTimer(ParticleDelayHandle, this, &ARevolver::SpawnEmitter, 0.2f, false, 0.2f);
			}
		}
	}
	else {
		SpawnEmitter();
	}
}

void ARevolver::SpawnEmitter()
{
	UGameplayStatics::SpawnEmitterAtLocation(this, FireExplosion, GunMesh->GetSocketLocation("Muzzle"), GetActorRotation(), FVector(0.1f, 0.1f, 0.1f));
	if (Cast<AMyCharacter>(CharacterActor) != NULL) {

		float VolumeControl = Cast<UMyProjectGameInstance>(GetWorld()->GetGameInstance())->VolumeControl;
		UGameplayStatics::PlaySound2D(GetWorld(), RevolverShot, VolumeControl, RevolverShot->GetPitchMultiplier());
	}
	else {
		float VolumeControl = Cast<UMyProjectGameInstance>(GetWorld()->GetGameInstance())->VolumeControl;
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), RevolverShot3D, GunMesh->GetSocketLocation("Muzzle"), VolumeControl, RevolverShot3D->GetPitchMultiplier());
	}
}

void ARevolver::OnEnterSphere(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<ACharacterBase>(OtherActor) != NULL) {

		ACharacterBase* CharacterEntered = Cast<ACharacterBase>(OtherActor);

		if ((CharacterEntered->GetHavePistol() == false) && (CharacterActor == nullptr)) {
			CharacterEntered->SetPistolActor(this);
			CharacterEntered->SetHavePistol(true);
			CharacterActor = CharacterEntered;
			this->AttachToComponent(CharacterEntered->GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("PistolSocket"));
			// Enable it's physics when character dies
			SphereCollision->SetSimulatePhysics(false);		

			if (Cast<AMyCharacter>(CharacterEntered) != NULL) {

				float VolumeControl = Cast<UMyProjectGameInstance>(GetWorld()->GetGameInstance())->VolumeControl;
				UGameplayStatics::PlaySound2D(GetWorld(), PickUp, VolumeControl, PickUp->GetPitchMultiplier());
			}
		}
		else {
			if ((Cast<AMyCharacter>(CharacterEntered) != NULL) && (CharacterActor == nullptr)) {
				AMyCharacter* MainChar = Cast<AMyCharacter>(CharacterEntered);
				if (MainChar->GetCurrPistolMagazine() < MainChar->GetPistolMagazineLimit()) {
					MainChar->SetCurrPistolMagazine(MainChar->GetPistolMagazineLimit());
					Destroy();

					float VolumeControl = Cast<UMyProjectGameInstance>(GetWorld()->GetGameInstance())->VolumeControl;
					UGameplayStatics::PlaySound2D(GetWorld(), PickUp, VolumeControl, PickUp->GetPitchMultiplier());
				}
			}
		}
	}
}




