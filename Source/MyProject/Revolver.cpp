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
	FVector SpawnLocation;

	if (Cast<AMyCharacter>(CharacterActor) != NULL) {

		AMyCharacter* MainCharacter = Cast<AMyCharacter>(CharacterActor);

		int32 ChanceToHit = FMath::FRandRange(1, 100);

		if (MainCharacter->GetZooming() == true) {

			SpawnLocation = MainCharacter->GetCamera()->GetComponentLocation() + (MainCharacter->GetCamera()->GetForwardVector() * 200);

			if (ChanceToHit < 30) {
				SpawnRotation = MainCharacter->GetCamera()->GetComponentRotation();
			}
			else {
				float BulletOffsetPitch;
				float BulletOffsetYaw;
				BulletOffsetPitch = FMath::RandRange(-ProjectileOffsetZoom, ProjectileOffsetZoom);
				BulletOffsetYaw = FMath::RandRange(-ProjectileOffsetZoom, ProjectileOffsetZoom);
				FRotator CurrRot = MainCharacter->GetCamera()->GetComponentRotation();
				SpawnRotation = FRotator(CurrRot.Pitch + BulletOffsetPitch, CurrRot.Yaw + BulletOffsetYaw, CurrRot.Roll);					}
		}
		else {

			SpawnLocation = MainCharacter->GetCamera()->GetComponentLocation() + (MainCharacter->GetCamera()->GetForwardVector() * 250);

			if (ChanceToHit < 15) {
				SpawnRotation = MainCharacter->GetCamera()->GetComponentRotation();
			}
			else {
				float BulletOffsetPitch;
				float BulletOffsetYaw;
				BulletOffsetPitch = FMath::RandRange(-ProjectileOffsetNoZoom, ProjectileOffsetNoZoom);
				BulletOffsetYaw = FMath::RandRange(-ProjectileOffsetNoZoom, ProjectileOffsetNoZoom);
				FRotator CurrRot = MainCharacter->GetCamera()->GetComponentRotation();
				SpawnRotation = FRotator(CurrRot.Pitch + BulletOffsetPitch, CurrRot.Yaw + BulletOffsetYaw, CurrRot.Roll);
			}
		}
	}
	else if (Cast<AGroundEnemy>(GetCharacterActor()) != NULL) {

		AGroundEnemy* EnemyCharacter = Cast<AGroundEnemy>(CharacterActor);

		int32 ChanceToHit = FMath::FRandRange(1, 100);

		SpawnLocation = EnemyCharacter->GetActorLocation() + (EnemyCharacter->GetActorForwardVector() * 100);

		if (ChanceToHit < 60) {
			SpawnRotation = EnemyCharacter->LookAtChar();
		}
		else {
			float BulletOffsetPitch;
			float BulletOffsetYaw;
			BulletOffsetPitch = FMath::RandRange(-ProjectileOffsetZoom, ProjectileOffsetZoom);
			BulletOffsetYaw = FMath::RandRange(-ProjectileOffsetZoom, ProjectileOffsetZoom);
			SpawnRotation = FRotator(EnemyCharacter->LookAtChar().Pitch + BulletOffsetPitch, EnemyCharacter->LookAtChar().Yaw + BulletOffsetYaw, EnemyCharacter->LookAtChar().Roll);
		}
	}

	FActorSpawnParameters ActorSpawnParams;
	AProjectile* SpawnedProjectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);

	SpawnedProjectile->SetCharacterActor(GetCharacterActor());
	SpawnedProjectile->SetDamage(Damage);

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

void ARevolver::OnEnterSphere(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (Cast<ACharacterBase>(OtherActor) != NULL) {

		ACharacterBase* CharacterEntered = Cast<ACharacterBase>(OtherActor);

		if (CharacterEntered->GetHavePistol() == false) {
			CharacterEntered->SetPistolActor(this);
			CharacterEntered->SetHavePistol(true);
			CharacterActor = CharacterEntered;
			this->AttachToComponent(CharacterEntered->GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("PistolSocket"));
			SphereCollision->SetSimulatePhysics(false);
		}
		else {
			if ((Cast<AMyCharacter>(CharacterEntered) != NULL) && (CharacterActor == NULL)) {
				AMyCharacter* MainChar = Cast<AMyCharacter>(CharacterEntered);
				if (MainChar->GetCurrPistolMagazine() < MainChar->GetPistolMagazineLimit()) {
					MainChar->SetCurrPistolMagazine(MainChar->GetPistolMagazineLimit());
					Destroy();
				}
			}
		}
	}
}




