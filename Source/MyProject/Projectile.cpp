// Fill out your copyright notice in the Description page of Project Settings.

#include "Projectile.h"
#include "MyCharacter.h"
#include "CharacterBase.h"
#include "GunBase.h"
#include "Revolver.h"
#include "Rifle.h"
#include "SaloonGroundEnemy.h"
#include "GroundEnemy.h"
#include "WindowEnemy.h"
#include "Windows.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Engine/GameEngine.h"
#include "DrawDebugHelpers.h"
#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Runtime/Engine/Public/EngineUtils.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Particles/ParticleSystem.h"
#include "Runtime/Engine/Classes/Particles/ParticleSystemComponent.h"


// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Damage = 0.0f;

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collision"));
	RootComponent = SphereCollision;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
	ProjectileMesh->SetupAttachment(RootComponent);

	LevelHandlerActor = nullptr;

	CharacterActor = nullptr;

	// Find the explosion particle asset in content browser by reference
	static ConstructorHelpers::FObjectFinder<UParticleSystem>
		ParticleSystem(TEXT("ParticleSystem'/Game/StarterContent/Particles/P_Explosion.P_Explosion'"));
	if (ParticleSystem.Succeeded() == true) {
		HitFire = ParticleSystem.Object;
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Fire Particle Not Found In Projectile!")));
	}

	// Find the blood splatter particle asset in content browser by reference
	static ConstructorHelpers::FObjectFinder<UParticleSystem>
		ParticleSystem2(TEXT("ParticleSystem'/Game/Assets/Particles/BloodSplatter.BloodSplatter'"));
	if (ParticleSystem2.Succeeded() == true) {
		HitBlood = ParticleSystem2.Object;
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Blood Particle Not Found In Projectile!")));
	}
	
	// Find the projectile trail particle asset in content browser by reference
	static ConstructorHelpers::FObjectFinder<UParticleSystem>
		ParticleSystem3(TEXT("ParticleSystem'/Game/Assets/Particles/ProjectileTrail.ProjectileTrail'"));
	if (ParticleSystem3.Succeeded() == true) {
		ProjectileTrail = ParticleSystem3.Object;
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Trail Not Found In Projectile!")));
	}
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	// Call OnHit when sphere hits something
	SphereCollision->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SpawnEmitter();
}

void AProjectile::SpawnEmitter()
{
	UGameplayStatics::SpawnEmitterAtLocation(this, ProjectileTrail, GetActorLocation(), GetActorRotation(), FVector(1.0f, 1.0f, 1.0f), true);
}

void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{

		if (OtherActor == CharacterActor) {
			Destroy();
		}
		if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && (OtherActor != CharacterActor)) {

			if (Cast<AMyCharacter>(CharacterActor) == NULL && Cast<AMyCharacter>(Hit.GetActor()) == NULL) {
				Destroy();		// If enemy hits another enemy bullet vanishes
			}
			else {
				ACharacterBase* HitActor = Cast<ACharacterBase>(Hit.GetComponent()->GetOwner());

				if (Hit.GetComponent()->IsA(USkeletalMeshComponent::StaticClass()) == true) {

					FVector BloodSplatterLoc = Hit.GetActor()->GetActorLocation() + (Hit.GetActor()->GetActorUpVector() * 20.0f);
					UGameplayStatics::SpawnEmitterAtLocation(this, HitBlood, BloodSplatterLoc, FRotator(0.0f, 0.0f, 0.0f), FVector(1.2f, 1.2f, 1.2f), true);

					// Headshot damage
					if ((Hit.BoneName.ToString() == "Head") || (Hit.BoneName.ToString() == "HeadTop_End")) {
						HitActor->SetHealth(HitActor->GetHealth() - Damage * 2.5f);
					}
					else {
						HitActor->SetHealth(HitActor->GetHealth() - Damage);
					}
					HitActor->SetIsHit(true);

					if (HitActor->GetHealth() <= 0) {
						// If Main Character dies play death animation and disable player input
						if (Cast<AMyCharacter>(HitActor) != NULL) {
							HitActor->PlayMainDeathAnim();
							HitActor->DisableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));
						}
						// If ground enemy dies it's pistol becomes retrievable
						else if (Cast<AGroundEnemy>(HitActor) != NULL) {
							AGroundEnemy* GroundEnemy = Cast<AGroundEnemy>(HitActor);
							GroundEnemy->GetPistolActor()->GetSphereCollision()->SetSimulatePhysics(true);
							GroundEnemy->GetPistolActor()->SetCharacterActor(nullptr);
							HitActor->PlayEnemyDeathAnim();
							GroundEnemy->DestroyAfterTime();
						}
						// If window enemy dies it's rifle becomes retrievable and flies off to the ground
						else if (Cast<AWindowEnemy>(HitActor) != NULL) {
							AWindowEnemy* WindowEnemy = Cast<AWindowEnemy>(HitActor);
							WindowEnemy->GetRifleActor()->GetSphereCollision()->SetSimulatePhysics(true);
							WindowEnemy->GetRifleActor()->GetSphereCollision()->AddForce(WindowEnemy->GetActorForwardVector() / GetWorld()->DeltaTimeSeconds * 800.0f * WindowEnemy->GetRifleActor()->GetSphereCollision()->GetMass());
							WindowEnemy->GetRifleActor()->SetCharacterActor(nullptr);
							HitActor->PlayEnemyDeathAnim();
							WindowEnemy->DestroyAfterTime();
						}
						// Everytime player makes a kill slow mo capacity increases and enemy animation plays
						HitActor->GetMainCharacterActor()->AddSlowMoCapacity(1.0f);
						HitActor->GetMesh()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
					}
				}
				else {
					UGameplayStatics::SpawnEmitterAtLocation(this, HitFire, Hit.Location, FRotator(0.0f, 0.0f, 0.0f), FVector(0.1f, 0.1f, 0.1f), true);
				}
			}
			Destroy();
		}
}

