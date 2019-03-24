// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

UCLASS()
class MYPROJECT_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	int32 Damage;

	UPROPERTY(VisibleAnywhere)
		class USphereComponent* SphereCollision;

	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* ProjectileMesh;

	UPROPERTY(VisibleAnywhere)
		class UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY()
		class ACharacterBase* CharacterActor;

	UPROPERTY()
		class UParticleSystem* HitFire;

	UPROPERTY()
		class UParticleSystem* HitBlood;

	UPROPERTY()
		class UParticleSystem* ProjectileTrail;

	UPROPERTY()
		class ALevelHandler* LevelHandlerActor;

	void SpawnEmitter();

	float DistanceToEnemy(ACharacterBase* HitEnemy);

	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetDamage(int32 Damage) { this->Damage = Damage; }

	void SetCharacterActor(ACharacterBase* CharacterActor) { this->CharacterActor = CharacterActor; }
};
