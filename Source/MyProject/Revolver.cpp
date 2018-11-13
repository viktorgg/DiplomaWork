// Fill out your copyright notice in the Description page of Project Settings.

#include "Revolver.h"
#include "Projectile.h"
#include "MyCharacter.h"
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
	FRotator SpawnRotation = CharacterRef->Camera->GetComponentRotation();
	FVector SpawnLocation;
	if (CharacterRef->bZooming == true) {
		SpawnLocation = CharacterRef->Camera->GetComponentLocation() + (CharacterRef->Camera->GetForwardVector() * 150);
	}
	else {
		SpawnLocation = CharacterRef->Camera->GetComponentLocation() + (CharacterRef->Camera->GetForwardVector() * 500);
	}
	FActorSpawnParameters ActorSpawnParams;
	// ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

	AProjectile* NewProjectile = GetWorld()->SpawnActor<AProjectile>(ProjectileRef, SpawnLocation, SpawnRotation, ActorSpawnParams);
	NewProjectile->CharacterRef = CharacterRef;

	UGameplayStatics::SpawnEmitterAtLocation(this, FireExplosion, GunMesh->GetSocketLocation("Muzzle"), GetActorRotation(), FVector(0.1f, 0.1f, 0.1f));
}

void ARevolver::OnEnterSphere(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if(Cast<AMyCharacter>(OtherActor) != NULL) {
		CharacterRef = Cast<AMyCharacter>(OtherActor);
		if (CharacterRef->bHavePistol == false) {
			CharacterRef->RevolverRef = this;
			this->AttachToComponent(CharacterRef->PlayerMesh, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("PistolSocket"));
			CharacterRef->bHavePistol = true;
		}
	}
}




