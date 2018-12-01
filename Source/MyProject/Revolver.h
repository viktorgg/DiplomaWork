// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GunBase.h"
#include "Revolver.generated.h"

UCLASS()
class MYPROJECT_API ARevolver : public AGunBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARevolver();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void SpawnProjectile();

private:
	
	virtual void SpawnEmitter();

	virtual void OnEnterSphere(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
