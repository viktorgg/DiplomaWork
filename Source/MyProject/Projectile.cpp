// Fill out your copyright notice in the Description page of Project Settings.

#include "Projectile.h"
#include "MyCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Engine/GameEngine.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"


// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collision"));
	RootComponent = SphereCollision;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
	ProjectileMesh->SetupAttachment(SphereCollision);

	Dmg = PistolDmg;
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	SphereCollision->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);

}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor == CharacterRef) {
		Destroy();
	}
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL)) {
		FVector Distance = OtherActor->GetActorLocation() - CharacterRef->GetActorLocation();

		LineTrace();
		
		Destroy();
	}
}

void AProjectile::LineTrace()
{
	FHitResult OutHit;

	FVector StartLoc = GetActorLocation() + (GetActorForwardVector() * 3);
	FVector EndLoc = (GetActorForwardVector() * 20.0f) + StartLoc;
	FCollisionQueryParams CollisionParams;

	if (GetWorld()->LineTraceSingleByChannel(OutHit, StartLoc, EndLoc, ECC_Camera, CollisionParams) != NULL) {

		if (OutHit.bBlockingHit == true) {

			if (OutHit.GetComponent()->IsA(USkeletalMeshComponent::StaticClass())) {

				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Enemy hit!")));

				AActor* HitActor = OutHit.GetComponent()->GetOwner();

				if (OutHit.BoneName.ToString() == "Head" || OutHit.BoneName.ToString() == "HeadTop_End" || OutHit.BoneName.ToString() == "Neck1") {
				
					GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Headshot!")));

					Cast<AMyCharacter>(HitActor)->Health -= Dmg * 4;
				}
				else {
					Cast<AMyCharacter>(HitActor)->Health -= Dmg;
				}
			}
		}
	}
}

