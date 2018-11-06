// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GunAbstract.generated.h"

UCLASS()
class MYPROJECT_API AGunAbstract : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGunAbstract();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
		class USphereComponent* SphereCollision;

	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* GunMesh;

	//UPROPERTY(VisibleAnywhere)
		//class ARevolver* RevolverRef;

	UPROPERTY(VisibleAnywhere)
		TSubclassOf<class AProjectile> ProjectileRef;

	UPROPERTY(VisibleAnywhere)
		class AMyCharacter* CharacterRef;
	
	UFUNCTION()
		virtual void SpawnProjectile();

	UFUNCTION()
		virtual void OnEnterSphere(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
