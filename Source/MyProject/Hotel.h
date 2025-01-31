// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuildingBase.h"
#include "Hotel.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API AHotel : public ABuildingBase
{
	GENERATED_BODY()
	
	
public:
	// Sets default values for this actor's properties
	AHotel();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void SpawnEnemy(int32 Place) override;

};
	
