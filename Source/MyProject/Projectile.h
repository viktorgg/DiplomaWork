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

	UPROPERTY(EditAnywhere)
		float BulletSpeed;

	int32 Damage;

	UPROPERTY(VisibleAnywhere)
		class USphereComponent* SphereCollision;

	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* ProjectileMesh;

	UPROPERTY(VisibleAnywhere)
		class ACharacterBase* CharacterActor;

	UPROPERTY(VisibleAnywhere)
		class UParticleSystem* HitFire;

	UPROPERTY(VisibleAnywhere)
		class UParticleSystem* HitBlood;

	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	void ProjectileTravel();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetDamage(int32 Damage) { this->Damage = Damage; }

	void SetCharacterActor(ACharacterBase* CharacterActor) { this->CharacterActor = CharacterActor; }

};
