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
#include "Runtime/Engine/Classes/Sound/SoundCue.h"
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

	CharacterActor = nullptr;

	// Find the projectile class in content browser by reference 
	static ConstructorHelpers::FClassFinder<AProjectile>
		ProjectileBP(TEXT("Blueprint'/Game/Blueprints/ProjectileBP.ProjectileBP_C'"));
	if (ProjectileBP.Succeeded() == true) {
		ProjectileClass = (UClass*)ProjectileBP.Class;
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Projectile Not Found In GunBase!")));
	}

	// Find the explosion particle asset in content browser by reference
	static ConstructorHelpers::FObjectFinder<UParticleSystem>
		ParticleSystem(TEXT("ParticleSystem'/Game/StarterContent/Particles/P_Explosion.P_Explosion'"));
	if (ParticleSystem.Succeeded() == true) {
		FireExplosion = ParticleSystem.Object;
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Particle Not Found In GunBase!")));
	}

	// Find the Revolver 3D cue in content browser by reference
	static ConstructorHelpers::FObjectFinder<USoundCue>
		CueAsset(TEXT("SoundCue'/Game/Assets/Sound/RevolverCue3D.RevolverCue3D'"));
	if (CueAsset.Succeeded() == true) {
		RevolverShot3D = CueAsset.Object;
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Revolvre 3D Cue Not Found In GunBase!")));
	}

	// Find the Rifle 3D cue in content browser by reference
	static ConstructorHelpers::FObjectFinder<USoundCue>
		CueAsset2(TEXT("SoundCue'/Game/Assets/Sound/RifleCue3D.RifleCue3D'"));
	if (CueAsset2.Succeeded() == true) {
		RifleShot3D = CueAsset2.Object;
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Rifle 3D Cue Not Found In GunBase!")));
	}

	// Find the Revolver cue in content browser by reference
	static ConstructorHelpers::FObjectFinder<USoundCue>
		CueAsset3(TEXT("SoundCue'/Game/Assets/Sound/RevolverCue.RevolverCue'"));
	if (CueAsset3.Succeeded() == true) {
		RevolverShot = CueAsset3.Object;
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Revolver Cue Not Found In GunBase!")));
	}

	// Find the Rifle cue in content browser by reference
	static ConstructorHelpers::FObjectFinder<USoundCue>
		CueAsset4(TEXT("SoundCue'/Game/Assets/Sound/RifleCue.RifleCue'"));
	if (CueAsset4.Succeeded() == true) {
		RifleShot = CueAsset4.Object;
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Rfile Cue Not Found In GunBase!")));
	}

	// Find the PickUp cue in content browser by reference
	static ConstructorHelpers::FObjectFinder<USoundCue>
		CueAsset5(TEXT("SoundCue'/Game/Assets/Sound/PickUpCue.PickUpCue'"));
	if (CueAsset5.Succeeded() == true) {
		PickUp = CueAsset5.Object;
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("PickUp Cue Not Found In GunBase!")));
	}
}

// Called when the game starts or when spawned
void AGunBase::BeginPlay()
{
	Super::BeginPlay();

	// Calls OnEnterSphere when something enters sphere
	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &AGunBase::OnEnterSphere);	

}

// Returns the rotation the projectile needs to be in order to hit exact spot player is aiming at
FRotator AGunBase::GetHitRot(FVector SocketLoc, class AMyCharacter* MainChar)
{
	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;

	FVector EndLoc = MainChar->GetCamera()->GetComponentLocation() + (MainChar->GetCamera()->GetForwardVector() * 1000000);

	// Fire a ray from camera to it's forward direction and save result in FHitResult struct
	GetWorld()->LineTraceSingleByChannel(HitResult, MainChar->GetCamera()->GetComponentLocation(), EndLoc, ECC_Camera, CollisionParams);

	if (HitResult.bBlockingHit) {
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







