// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuildingBase.h"
#include "NationalBank.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API ANationalBank : public ABuildingBase
{
	GENERATED_BODY()
	

public:
	// Sets default values for this actor's properties
	ANationalBank();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
};
