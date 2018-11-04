// Fill out your copyright notice in the Description page of Project Settings.

#include "GunAbstract.h"
#include "Projectile.h"
#include "MyCharacter.h"
#include "Revolver.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Engine/GameEngine.h"
#include "Kismet/GameplayStatics.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "EngineUtils.h"

// Sets default values
AGunAbstract::AGunAbstract()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collision"));
	RootComponent = SphereCollision;

	GunMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Gun Mesh"));
	GunMesh->SetupAttachment(SphereCollision);

	static ConstructorHelpers::FClassFinder<AProjectile>
		ProjectileBP(TEXT("Blueprint'/Game/Blueprints/ProjectileBP.ProjectileBP_C'"));
	if (ProjectileBP.Class) {
		Projectile = (UClass*)ProjectileBP.Class;
	}

}

// Called when the game starts or when spawned
void AGunAbstract::BeginPlay()
{
	Super::BeginPlay();
	
	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &AGunAbstract::OnEnterSphere);

	Character = Cast<AMyCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	for (TActorIterator<ARevolver> ActorItr(GetWorld()); ActorItr; ++ActorItr) {
		Revolver = *ActorItr;
	}
}

// Called every frame
void AGunAbstract::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGunAbstract::SpawnProjectile()
{
}

void AGunAbstract::OnEnterSphere(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
}


