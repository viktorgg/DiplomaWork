// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BuildingBase.generated.h"

UCLASS()
class MYPROJECT_API ABuildingBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABuildingBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


private:

	UPROPERTY(VisibleAnywhere)
		TArray<TSubclassOf<class AWindows>> WindowsArray;

	TSubclassOf<class AWindows> WindowsRef;

	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* MainBuildingMesh;

	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* WallMesh;

	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* FirstFloorMesh;

	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* SecondFloorMesh;
	
	
};
