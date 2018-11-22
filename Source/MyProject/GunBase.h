// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GunBase.generated.h"

UCLASS()
class MYPROJECT_API AGunBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGunBase();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	

private:

	UPROPERTY(EditAnywhere)
		int32 Damage;

	UPROPERTY(EditAnywhere)
		float FireRate;

	UPROPERTY(EditAnywhere)
		float ProjectileOffsetNoZoom;

	UPROPERTY(EditAnywhere)
		float ProjectileOffsetZoom;

	UPROPERTY(VisibleAnywhere)
		class USphereComponent* SphereCollision;

	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* GunMesh;

	UPROPERTY(VisibleAnywhere)
		TSubclassOf<class AProjectile> ProjectileRef;

	UPROPERTY(VisibleAnywhere)
		class ACharacterBase* CharacterRef;

	UPROPERTY(VisibleAnywhere)
		class UParticleSystem* FireExplosion;

	virtual void SpawnProjectile() PURE_VIRTUAL(AGunBase::SpawnProjectile, );

	UFUNCTION()
		virtual void OnEnterSphere(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) PURE_VIRTUAL(AGunBase::OnEnterSphere, );

public:

	int32 GetDamage() const { return Damage; }
	void SetDamage(int32 Input) { Damage = Input; }

	float GetFireRate() const { return FireRate; }
	void SetFireRate(float Input) { FireRate = Input; }

	float GetProjectileOffsetNoZoom() const { return ProjectileOffsetNoZoom; }
	void SetProjectileOffsetNoZoom(float Input) { ProjectileOffsetNoZoom = Input; }

	float GetProjectileOffsetZoom() const { return ProjectileOffsetZoom; }
	void SetProjectileOffsetZoom(float Input) { ProjectileOffsetZoom = Input; }

	ACharacterBase* GetCharacterRef() const { return CharacterRef; }

	TSubclassOf<AProjectile> GetProjectileRef() const { return ProjectileRef; }

	void SetCharacterRef(ACharacterBase* Input) { CharacterRef = Input; }

	UStaticMeshComponent* GetGunMesh() const { return GunMesh; }

	UParticleSystem* GetFireExplosion() const { return FireExplosion; }
};
