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

//-----------------------------------

	UPROPERTY(VisibleAnywhere)
		class USphereComponent* SphereCollision;

	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* GunMesh;

	UPROPERTY(VisibleAnywhere)
		TSubclassOf<class AProjectile> ProjectileRef;

	UPROPERTY(VisibleAnywhere)
		class AProjectile* LastSpawnedProjectile;

	UPROPERTY(VisibleAnywhere)
		class AMyCharacter* CharacterRef;
	
//-----------------------------------

	UFUNCTION()
		virtual void SpawnProjectile() PURE_VIRTUAL(AGunBase::SpawnProjectile,);

	UFUNCTION()
		virtual void OnEnterSphere(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) PURE_VIRTUAL(AGunBase::OnEnterSphere,);
};
