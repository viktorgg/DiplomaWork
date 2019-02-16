// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GunBase.generated.h"

UCLASS(Abstract)
class MYPROJECT_API AGunBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGunBase();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FTimerHandle ParticleDelayHandle;

	UPROPERTY(EditAnywhere)
		int32 Damage;

	UPROPERTY(EditAnywhere)
		float ProjectileOffsetNoZoom;

	UPROPERTY(EditAnywhere)
		float ProjectileOffsetZoom;

	UPROPERTY(VisibleAnywhere)
		class USphereComponent* SphereCollision;

	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* GunMesh;

	UPROPERTY(VisibleAnywhere)
		UClass* ProjectileClass;

	UPROPERTY(VisibleAnywhere)
		class ACharacterBase* CharacterActor;

	UPROPERTY(VisibleAnywhere)
		class UParticleSystem* FireExplosion;

	class USoundCue* RevolverShot3D;
	class USoundCue* RevolverShot;
	class USoundCue* RifleShot3D;
	class USoundCue* RifleShot;
	class USoundCue* PickUp;

	virtual void SpawnProjectile() PURE_VIRTUAL(AGunBase::SpawnProjectile, );

	virtual void SpawnEmitter() PURE_VIRTUAL(AGunBase::SpawnEmitter, );

	FRotator GetHitRot(FVector SocketLoc, class AMyCharacter* MainChar);

	UFUNCTION()
		virtual void OnEnterSphere(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) PURE_VIRTUAL(AGunBase::OnEnterSphere, );


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	

public:

	int32 GetDamage() const { return Damage; }
	void SetDamage(int32 Damage) { this->Damage = Damage; }

	float GetProjectileOffsetNoZoom() const { return ProjectileOffsetNoZoom; }
	void SetProjectileOffsetNoZoom(float ProjectileOffset) { this->ProjectileOffsetNoZoom = ProjectileOffset; }

	float GetProjectileOffsetZoom() const { return ProjectileOffsetZoom; }
	void SetProjectileOffsetZoom(float ProjectileOffset) { this->ProjectileOffsetZoom = ProjectileOffset; }

	ACharacterBase* GetCharacterActor() const { return CharacterActor; }

	UClass* GetProjectileClass() const { return ProjectileClass; }

	void SetCharacterActor(ACharacterBase* CharacterActor) { this->CharacterActor = CharacterActor; }

	UStaticMeshComponent* GetGunMesh() const { return GunMesh; }

	USphereComponent* GetSphereCollision() const { return SphereCollision; }

	UParticleSystem* GetFireExplosion() const { return FireExplosion; }
};
