// Fill out your copyright notice in the Description page of Project Settings.

#include "Revolver.h"
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
#include "EngineUtils.h"


// Sets default values
ARevolver::ARevolver()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Damage = 20;
	FireRate = 0.25f;
	ProjectileOffset = 1.0f;
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

	if (Cast<AMyCharacter>(CharacterRef) != NULL) {
		AMyCharacter* MainCharacter = Cast<AMyCharacter>(CharacterRef);
		if (MainCharacter->bZooming == true) {
			SpawnRotation = MainCharacter->Camera->GetComponentRotation();
			SpawnLocation = MainCharacter->Camera->GetComponentLocation() + (MainCharacter->Camera->GetForwardVector() * 200);
		}
		else {
			float BulletOffset;
			BulletOffset = FMath::RandRange(-ProjectileOffset, ProjectileOffset);
			SpawnLocation = GunMesh->GetSocketLocation("Muzzle");
			FRotator CurrRot = MainCharacter->Camera->GetComponentRotation();
			SpawnRotation = FRotator(CurrRot.Pitch + BulletOffset, CurrRot.Yaw + BulletOffset, CurrRot.Roll);
		}
	}
	FActorSpawnParameters ActorSpawnParams;
	// ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

	AProjectile* NewProjectile = GetWorld()->SpawnActor<AProjectile>(ProjectileRef, SpawnLocation, SpawnRotation, ActorSpawnParams);

	NewProjectile->CharacterRef = CharacterRef;
	NewProjectile->Damage = Damage;

	UGameplayStatics::SpawnEmitterAtLocation(this, FireExplosion, GunMesh->GetSocketLocation("Muzzle"), GetActorRotation(), FVector(0.1f, 0.1f, 0.1f));
}

void ARevolver::OnEnterSphere(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (Cast<ACharacterBase>(OtherActor) != NULL) {
		ACharacterBase* CharacterEntered = Cast<ACharacterBase>(OtherActor);
		if (CharacterEntered->bHavePistol == false) {
			CharacterEntered->RevolverRef = this;
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("%s"), *CharacterEntered->RevolverRef->GetName()));
			this->AttachToComponent(CharacterEntered->GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("PistolSocket"));
			CharacterEntered->bHavePistol = true;
			CharacterEntered->RevolverFireRate = FireRate;
		}
	}
}




