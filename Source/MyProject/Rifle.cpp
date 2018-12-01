// Fill out your copyright notice in the Description page of Project Settings.

#include "Rifle.h"
#include "Projectile.h"
#include "MyCharacter.h"
#include "CharacterBase.h"
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

	SetDamage(50);
	SetFireRate(2.0f);
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
	FVector SpawnLocation;

	if (Cast<AMyCharacter>(GetCharacterRef()) != NULL) {

		AMyCharacter* MainCharacter = Cast<AMyCharacter>(GetCharacterRef());

		int32 ChanceToHit = FMath::FRandRange(1, 100);

		if (MainCharacter->GetZooming() == true) {

			SpawnLocation = MainCharacter->GetCamera()->GetComponentLocation() + (MainCharacter->GetCamera()->GetForwardVector() * 200);
			SpawnRotation = MainCharacter->GetCamera()->GetComponentRotation();
		}
		else {

			SpawnLocation = MainCharacter->GetCamera()->GetComponentLocation() + (MainCharacter->GetCamera()->GetForwardVector() * 250);

			if (ChanceToHit < 35) {
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
	FActorSpawnParameters ActorSpawnParams;
	// ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

	AProjectile* SpawnedProjectile = GetWorld()->SpawnActor<AProjectile>(GetProjectileRef(), SpawnLocation, SpawnRotation, ActorSpawnParams);

	SpawnedProjectile->SetCharacterRef(GetCharacterRef());
	SpawnedProjectile->SetDamage(GetDamage());

	if (Cast<AMyCharacter>(GetCharacterRef())->GetZooming() == true) {
		SpawnEmitter();
	}
	else {
		GetWorldTimerManager().SetTimer(GetParticleDelayHandle(), this, &ARifle::SpawnEmitter, 0.2f, false, 0.2f);
	}
}

void ARifle::SpawnEmitter()
{
	UGameplayStatics::SpawnEmitterAtLocation(this, GetFireExplosion(), GetGunMesh()->GetSocketLocation("Muzzle"), GetActorRotation(), FVector(0.1f, 0.1f, 0.1f));
}

void ARifle::OnEnterSphere(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (Cast<ACharacterBase>(OtherActor) != NULL) {
		ACharacterBase* CharacterEntered = Cast<ACharacterBase>(OtherActor);
		if (CharacterEntered->GetHaveRifle() == false) {
			CharacterEntered->SetRifleRef(this);
			CharacterEntered->SetHaveRifle(true);
			SetCharacterRef(CharacterEntered);
			this->AttachToComponent(CharacterEntered->GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("RifleSocket"));
			CharacterEntered->SetRifleFireRate(GetFireRate());
		}
	}
}
