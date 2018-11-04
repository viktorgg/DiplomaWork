// Fill out your copyright notice in the Description page of Project Settings.

#include "Revolver.h"
#include "Projectile.h"
#include "MyCharacter.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
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
	const FRotator SpawnRotation = GunMesh->GetRelativeTransform().GetRotation().Rotator();
	const FVector SpawnLocation = GunMesh->GetSocketLocation("Muzzle");
	FActorSpawnParameters ActorSpawnParams;
	// ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

	GetWorld()->SpawnActor<AProjectile>(Projectile, SpawnLocation, SpawnRotation, ActorSpawnParams);
}

void ARevolver::OnEnterSphere(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if(OtherActor == Character) {
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Entered!")));
	// Revolver->AttachToComponent(Character->PlayerMesh, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("RightHandMiddle1"));
	Revolver->AttachToComponent(Character->PlayerMesh, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("WeaponSocket"));
	}
}




