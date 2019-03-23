// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BuildingBase.h"
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

	virtual void PostInitializeComponents() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Array to store structure of every enemy
	TArray<TSharedPtr<FEnemyHandler>> SEnemyHandlerArray;

	void SpawnEnemy(int32 Place);

private:
	
	UPROPERTY()
		UClass* WindowEnemyClass;
	UPROPERTY()
		UClass* SaloonGroundEnemyClass;

	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* MainBuildingMesh;

	UPROPERTY(VisibleAnywhere)
		class UChildActorComponent* DoorsChild;

public:

	TArray<TSharedPtr<FEnemyHandler>> GetSEnemyHandlerArray() const { return SEnemyHandlerArray; }

};
