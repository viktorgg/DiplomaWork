// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SaloonBuilding.generated.h"

UCLASS()
class MYPROJECT_API ASaloonBuilding : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASaloonBuilding();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* MainBuildingMesh;

	UPROPERTY(VisibleAnywhere)
		class UBoxComponent* BoxCollision;

	UPROPERTY(VisibleAnywhere)
		class UBoxComponent* BoxCollision2;

	UPROPERTY(VisibleAnywhere)
		class UBoxComponent* BoxCollision3;

	UPROPERTY(VisibleAnywhere)
		class UBoxComponent* BoxCollision4;

	UPROPERTY(VisibleAnywhere)
		class UBoxComponent* BoxCollision5;

	UPROPERTY(VisibleAnywhere)
		class UBoxComponent* BoxCollision6;
	
};
