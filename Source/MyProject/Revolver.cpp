// Fill out your copyright notice in the Description page of Project Settings.

#include "Revolver.h"
#include "Projectile.h"
#include "MyCharacter.h"
#include "CharacterBase.h"
#include "GroundEnemy.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Camera/CameraComponent.h"
#include "Engine/GameEngine.h"
#include "Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Public/TimerManager.h"
#include "EngineUtils.h"


// Sets default values
ARevolver::ARevolver()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Damage = 20;
	SetProjectileOffsetNoZoom(2.5f);
	SetProjectileOffsetZoom(1.5f);
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

			if (ChanceToHit < 30) {		// There's a 30% chance the bullet will go exactly at crosshair when zooming
				SpawnRotation = GetHitRot(SpawnLocation, MainCharacter);
			}
			else {
				float BulletOffsetPitch;
				float BulletOffsetYaw;
				BulletOffsetPitch = FMath::RandRange(-ProjectileOffsetZoom, ProjectileOffsetZoom);		// Add a random deviation to the crosshair's location
				BulletOffsetYaw = FMath::RandRange(-ProjectileOffsetZoom, ProjectileOffsetZoom);
				FRotator CurrRot = GetHitRot(SpawnLocation, MainCharacter);
				SpawnRotation = FRotator(CurrRot.Pitch + BulletOffsetPitch, CurrRot.Yaw + BulletOffsetYaw, CurrRot.Roll);	
			}
		}
		else {
			if (ChanceToHit < 15) {		// There's a 15% chance the bullet will go exactly at crosshair when not zooming
				SpawnRotation = GetHitRot(SpawnLocation, MainCharacter);
			}
			else {
				float BulletOffsetPitch;
				float BulletOffsetYaw;
				BulletOffsetPitch = FMath::RandRange(-ProjectileOffsetNoZoom, ProjectileOffsetNoZoom);		// Add an even bigger random deviation to the crosshair's location
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

		// There's a 60% chance the bullet will go exactly at player's character
		if (ChanceToHit < 60) {		
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
		if (Cast<AMyCharacter>(CharacterActor)->GetZooming() == true) {
			SpawnEmitter();
		}
		else {
			GetWorldTimerManager().SetTimer(ParticleDelayHandle, this, &ARevolver::SpawnEmitter, 0.2f, false, 0.2f);
		}
	}
	else {
		SpawnEmitter();
	}
}

void ARevolver::SpawnEmitter()
{
	UGameplayStatics::SpawnEmitterAtLocation(this, FireExplosion, GunMesh->GetSocketLocation("Muzzle"), GetActorRotation(), FVector(0.1f, 0.1f, 0.1f));
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
			SphereCollision->SetSimulatePhysics(false);		// Enable it's physics when character dies
		}
		else {
			if ((Cast<AMyCharacter>(CharacterEntered) != NULL) && (CharacterActor == nullptr)) {
				AMyCharacter* MainChar = Cast<AMyCharacter>(CharacterEntered);
				if (MainChar->GetCurrPistolMagazine() < MainChar->GetPistolMagazineLimit()) {
					MainChar->SetCurrPistolMagazine(MainChar->GetPistolMagazineLimit());
					Destroy();
				}
			}
		}
	}
}




