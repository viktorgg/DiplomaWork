// Fill out your copyright notice in the Description page of Project Settings.

#include "Projectile.h"
#include "MyCharacter.h"
#include "CharacterBase.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Engine/GameEngine.h"
#include "DrawDebugHelpers.h"
#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Particles/ParticleSystemComponent.h"


// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BulletSpeed = 100.0f;

	Damage = 0.0f;

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collision"));
	RootComponent = SphereCollision;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
	ProjectileMesh->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UParticleSystem>
		ParticleSystem(TEXT("ParticleSystem'/Game/StarterContent/Particles/P_Explosion.P_Explosion'"));
	if (ParticleSystem.Succeeded() == true) {
		HitSmoke = ParticleSystem.Object;
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Particle Not Found!")));
	}

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

	ProjectileTravel();
}

void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor == CharacterRef) {
		Destroy();
	}

	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL) && (OtherActor != CharacterRef)) {

		FVector DistanceVector = OtherActor->GetActorLocation() - CharacterRef->GetActorLocation();
		float Distance = DistanceVector.Size();

		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("%f"), Distance));

		ACharacterBase* HitActor = Cast<ACharacterBase>(Hit.GetComponent()->GetOwner());

		if (Hit.GetComponent()->IsA(USkeletalMeshComponent::StaticClass()) == true) {

			if (Hit.BoneName.ToString() == "Head" || Hit.BoneName.ToString() == "HeadTop_End" || Hit.BoneName.ToString() == "Neck1") {

				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Headshot!")));

				HitActor->SetHealth(HitActor->GetHealth() - Damage * 2.5);
			}
			else {

				HitActor->SetHealth(HitActor->GetHealth() - Damage);
			}
		}
		else {
			UGameplayStatics::SpawnEmitterAtLocation(this, HitSmoke, Hit.Location, FRotator(0.0f, 0.0f, 0.0f), FVector(0.1f, 0.1f, 0.1f), true);
		}
		
		Destroy();
	}
}

void AProjectile::ProjectileTravel()
{
	SetActorLocation(FMath::VInterpConstantTo(GetActorLocation(), GetActorLocation() + (GetActorForwardVector() * 20000.0f),GetWorld()->DeltaTimeSeconds, BulletSpeed));
}

void AProjectile::LineTrace()
{
	FHitResult OutHit;

	FVector StartLoc = GetActorLocation() + (GetActorForwardVector() * 5.0f);
	FVector EndLoc = (GetActorForwardVector() * 60.0f) + StartLoc;
	FCollisionQueryParams CollisionParams;

	DrawDebugLine(GetWorld(), GetActorLocation() + (GetActorForwardVector() * 5.0f), (GetActorForwardVector() * 60.0f) + StartLoc, FColor::Emerald, true, 999, 0, 10);

	if (GetWorld()->LineTraceSingleByChannel(OutHit, StartLoc, EndLoc, ECC_Camera, CollisionParams) != NULL) {

		if (OutHit.bBlockingHit == true) {
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Tracing!")));
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Hit: %s"), *OutHit.GetComponent()->GetName()));
			if (OutHit.GetComponent()->IsA(USkeletalMeshComponent::StaticClass())) {

				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Enemy hit!")));

				ACharacterBase* HitActor = Cast<ACharacterBase>(OutHit.GetComponent()->GetOwner());

				if (OutHit.BoneName.ToString() == "Head" || OutHit.BoneName.ToString() == "HeadTop_End" || OutHit.BoneName.ToString() == "Neck1") {
				
					GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Headshot!")));

					HitActor->SetHealth(HitActor->GetHealth() - 80);
				}
				else {
					HitActor->SetHealth(HitActor->GetHealth() - 20);
				}
			}
		}
	}
}

