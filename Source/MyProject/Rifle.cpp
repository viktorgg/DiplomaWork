// Fill out your copyright notice in the Description page of Project Settings.

#include "Rifle.h"
#include "Projectile.h"
#include "MyCharacter.h"
#include "CharacterBase.h"
#include "WindowEnemy.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Camera/CameraComponent.h"
#include "Engine/GameEngine.h"
#include "Kismet/GameplayStatics.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Public/TimerManager.h"
#include "EngineUtils.h"


ARifle::ARifle() {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Damage = 50;
	SetProjectileOffsetNoZoom(2.5f);
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

			if (ChanceToHit < 35) {
				SpawnRotation = GetHitRot(SpawnLocation, MainCharacter);
			}
			else {
				float BulletOffsetPitch;
				float BulletOffsetYaw;
				BulletOffsetPitch = FMath::RandRange(-ProjectileOffsetNoZoom, ProjectileOffsetNoZoom);
				BulletOffsetYaw = FMath::RandRange(-ProjectileOffsetNoZoom, ProjectileOffsetNoZoom);
				FRotator CurrRot = GetHitRot(SpawnLocation, MainCharacter);
				SpawnRotation = FRotator(CurrRot.Pitch + BulletOffsetPitch, CurrRot.Yaw + BulletOffsetYaw, CurrRot.Roll);
			}
		}
	}
	else if (Cast<AWindowEnemy>(CharacterActor) != NULL) {

		AWindowEnemy* EnemyCharacter = Cast<AWindowEnemy>(CharacterActor);

		int32 ChanceToHit = FMath::FRandRange(1, 100);

		if (ChanceToHit < 30) {
			SpawnRotation = EnemyCharacter->LookAtChar();
		}
		else {
			float BulletOffsetPitch;
			float BulletOffsetYaw;
			BulletOffsetPitch = FMath::RandRange(-ProjectileOffsetNoZoom * 2.5f, ProjectileOffsetNoZoom * 2.5f);
			BulletOffsetYaw = FMath::RandRange(-ProjectileOffsetNoZoom * 2.5f, ProjectileOffsetNoZoom * 2.5f);
			SpawnRotation = FRotator(EnemyCharacter->LookAtChar().Pitch + BulletOffsetPitch, EnemyCharacter->LookAtChar().Yaw + BulletOffsetYaw, EnemyCharacter->LookAtChar().Roll);
		}
	}
	FActorSpawnParameters ActorSpawnParams;
	AProjectile* SpawnedProjectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);

	SpawnedProjectile->SetCharacterActor(CharacterActor);
	SpawnedProjectile->SetDamage(Damage);

	if (Cast<AMyCharacter>(CharacterActor) != NULL) {
		if (Cast<AMyCharacter>(CharacterActor)->GetZooming() == true) {
			SpawnEmitter();
		}
		else {
			GetWorldTimerManager().SetTimer(ParticleDelayHandle, this, &ARifle::SpawnEmitter, 0.2f, false, 0.2f);
		}
	}
	else {
		SpawnEmitter();
	}
}

void ARifle::SpawnEmitter()
{
	UGameplayStatics::SpawnEmitterAtLocation(this, FireExplosion, GunMesh->GetSocketLocation("Muzzle"), GetActorRotation(), FVector(0.1f, 0.1f, 0.1f));
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
			SphereCollision->SetSimulatePhysics(false);
		}
		else {
			if ((Cast<AMyCharacter>(CharacterEntered) != NULL) && (CharacterActor == nullptr)) {
				AMyCharacter* MainChar = Cast<AMyCharacter>(CharacterEntered);
				if (MainChar->GetCurrRifleMagazine() < MainChar->GetRifleMagazineLimit()) {
					MainChar->SetCurrRifleMagazine(MainChar->GetRifleMagazineLimit());
					Destroy();
				}
			}
		}
	}
}
