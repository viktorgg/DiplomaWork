// Fill out your copyright notice in the Description page of Project Settings.

#include "GunBase.h"
#include "Projectile.h"
#include "MyCharacter.h"
#include "Revolver.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Engine/GameEngine.h"
#include "Kismet/GameplayStatics.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Particles/ParticleSystemComponent.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"
#include "EngineUtils.h"

// Sets default values
AGunBase::AGunBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Damage = 0;
	ProjectileOffsetNoZoom = 0.0f;
	ProjectileOffsetZoom = 0.0f;

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collision"));
	RootComponent = SphereCollision;

	GunMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Gun Mesh"));
	GunMesh->SetupAttachment(RootComponent);

	// Find the projectile class in content browser by reference 
	static ConstructorHelpers::FClassFinder<AProjectile>
		ProjectileBP(TEXT("Blueprint'/Game/Blueprints/ProjectileBP.ProjectileBP_C'"));
	if (ProjectileBP.Succeeded() == true) {
		ProjectileClass = (UClass*)ProjectileBP.Class;
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Projectile Not Found!")));
	}

	// Find the explosion particle asset in content browser by reference
	static ConstructorHelpers::FObjectFinder<UParticleSystem>
		ParticleSystem(TEXT("ParticleSystem'/Game/StarterContent/Particles/P_Explosion.P_Explosion'"));
	if (ParticleSystem.Succeeded() == true) {
		FireExplosion = ParticleSystem.Object;
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Particle Not Found!")));
	}

}

// Called when the game starts or when spawned
void AGunBase::BeginPlay()
{
	Super::BeginPlay();

	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &AGunBase::OnEnterSphere);	// Calls OnEnterSphere when something enters sphere

}

// Returns the rotation the projectile needs to be in order to hit exact spot player is aiming at
FRotator AGunBase::GetHitRot(FVector SocketLoc, class AMyCharacter* MainChar)
{
	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;

	FVector EndLoc = MainChar->GetCamera()->GetComponentLocation() + (MainChar->GetCamera()->GetForwardVector() * 1000000);

	// Fire a ray from camera to it's forward direction and save result in FHitResult struct
	GetWorld()->LineTraceSingleByChannel(HitResult, MainChar->GetCamera()->GetComponentLocation(), EndLoc, ECC_Camera, CollisionParams);

	if (HitResult.bBlockingHit == true) {
		return UKismetMathLibrary::FindLookAtRotation(SocketLoc, HitResult.ImpactPoint);
	}
	else {
		return MainChar->GetCamera()->GetComponentRotation();
	}
}

// Called every frame
void AGunBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}







