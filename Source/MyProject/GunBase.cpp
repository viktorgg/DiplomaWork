// Fill out your copyright notice in the Description page of Project Settings.

#include "GunBase.h"
#include "Projectile.h"
#include "MyCharacter.h"
#include "Revolver.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Engine/GameEngine.h"
#include "Kismet/GameplayStatics.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Particles/ParticleSystemComponent.h"
#include "EngineUtils.h"

// Sets default values
AGunBase::AGunBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collision"));
	RootComponent = SphereCollision;

	GunMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Gun Mesh"));
	GunMesh->SetupAttachment(SphereCollision);

	static ConstructorHelpers::FClassFinder<AProjectile>
		ProjectileBP(TEXT("Blueprint'/Game/Blueprints/ProjectileBP.ProjectileBP_C'"));
	if (ProjectileBP.Succeeded() == true) {
		ProjectileRef = (UClass*)ProjectileBP.Class;
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Projectile Not Found!")));
	}

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

	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &AGunBase::OnEnterSphere);

}

// Called every frame
void AGunBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}







