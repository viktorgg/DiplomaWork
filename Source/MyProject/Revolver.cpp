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

	SetDamage(20);
	SetFireRate(0.25);
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

	if (Cast<AMyCharacter>(GetCharacterRef()) != NULL) {

		AMyCharacter* MainCharacter = Cast<AMyCharacter>(GetCharacterRef());

		int32 ChanceToHit = FMath::FRandRange(1, 100);

		if (MainCharacter->GetZooming() == true) {

			SpawnLocation = MainCharacter->GetCamera()->GetComponentLocation() + (MainCharacter->GetCamera()->GetForwardVector() * 200);

			if (ChanceToHit < 30) {
				SpawnRotation = MainCharacter->GetCamera()->GetComponentRotation();
			}
			else {
				float BulletOffsetPitch;
				float BulletOffsetYaw;
				BulletOffsetPitch = FMath::RandRange(-GetProjectileOffsetZoom(), GetProjectileOffsetZoom());
				BulletOffsetYaw = FMath::RandRange(-GetProjectileOffsetZoom(), GetProjectileOffsetZoom());
				FRotator CurrRot = MainCharacter->GetCamera()->GetComponentRotation();
				SpawnRotation = FRotator(CurrRot.Pitch + BulletOffsetPitch, CurrRot.Yaw + BulletOffsetYaw, CurrRot.Roll);
			}
		}
		else {
			
			SpawnLocation = MainCharacter->GetCamera()->GetComponentLocation() + (MainCharacter->GetCamera()->GetForwardVector() * 250);

			if (ChanceToHit < 15) {
				SpawnRotation = MainCharacter->GetCamera()->GetComponentRotation();
			}
			else {
				float BulletOffsetPitch;
				float BulletOffsetYaw;
				BulletOffsetPitch = FMath::RandRange(-GetProjectileOffsetNoZoom(), GetProjectileOffsetNoZoom());
				BulletOffsetYaw = FMath::RandRange(-GetProjectileOffsetNoZoom(), GetProjectileOffsetNoZoom());
				FRotator CurrRot = MainCharacter->GetCamera()->GetComponentRotation();
				SpawnRotation = FRotator(CurrRot.Pitch + BulletOffsetPitch, CurrRot.Yaw + BulletOffsetYaw, CurrRot.Roll);
			}
		}
	}
	else if (Cast<AGroundEnemy>(GetCharacterRef()) != NULL) {

		AGroundEnemy* EnemyCharacter = Cast<AGroundEnemy>(GetCharacterRef());

		int32 ChanceToHit = FMath::FRandRange(1, 100);

		SpawnLocation = EnemyCharacter->GetActorLocation() + (EnemyCharacter->GetActorForwardVector() * 100);

		if (ChanceToHit < 60) {
			SpawnRotation = EnemyCharacter->LookAtRot();
		}
		else {
			float BulletOffsetPitch;
			float BulletOffsetYaw;
			BulletOffsetPitch = FMath::RandRange(-GetProjectileOffsetZoom(), GetProjectileOffsetZoom());
			BulletOffsetYaw = FMath::RandRange(-GetProjectileOffsetZoom(), GetProjectileOffsetZoom());
			FRotator CurrRot = EnemyCharacter->GetActorRotation();
			SpawnRotation = FRotator(CurrRot.Pitch + BulletOffsetPitch, CurrRot.Yaw + BulletOffsetYaw, CurrRot.Roll);
		}
	}
	FActorSpawnParameters ActorSpawnParams;
	// ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

	AProjectile* SpawnedProjectile = GetWorld()->SpawnActor<AProjectile>(GetProjectileRef(), SpawnLocation, SpawnRotation, ActorSpawnParams);

	SpawnedProjectile->SetCharacterRef(GetCharacterRef());
	SpawnedProjectile->SetDamage(GetDamage());

	if (Cast<AMyCharacter>(GetCharacterRef())->GetZooming() == true){
		SpawnEmitter();
	}
	else {
		GetWorldTimerManager().SetTimer(GetParticleDelayHandle(), this, &ARevolver::SpawnEmitter, 0.2f, false, 0.2f);
	}
}

void ARevolver::SpawnEmitter()
{
	UGameplayStatics::SpawnEmitterAtLocation(this, GetFireExplosion(), GetGunMesh()->GetSocketLocation("Muzzle"), GetActorRotation(), FVector(0.1f, 0.1f, 0.1f));
}

void ARevolver::OnEnterSphere(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (Cast<ACharacterBase>(OtherActor) != NULL) {
		ACharacterBase* CharacterEntered = Cast<ACharacterBase>(OtherActor);
		if (CharacterEntered->GetHavePistol() == false) {
			CharacterEntered->SetPistolRef(this);
			CharacterEntered->SetHavePistol(true);
			SetCharacterRef(CharacterEntered);
			this->AttachToComponent(CharacterEntered->GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("PistolSocket"));
			GetSphereCollision()->SetSimulatePhysics(false);
			CharacterEntered->SetPistolFireRate(GetFireRate());
		}
	}
}




