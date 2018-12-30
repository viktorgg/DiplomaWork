// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuildingBase.h"
#include "GeneralStore.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API AGeneralStore : public ABuildingBase
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AGeneralStore();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
};
