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

	TArray<class AWindows*> WindowsArray;

	UPROPERTY(VisibleAnywhere)
		class UChildActorComponent* WindowsChild;

	UPROPERTY(VisibleAnywhere)
		class UChildActorComponent* WindowsChild2;

	UPROPERTY(VisibleAnywhere)
		class UChildActorComponent* WindowsChild3;

	UPROPERTY(VisibleAnywhere)
		class UChildActorComponent* WindowsChild4;

	TSubclassOf<class AWindows> WindowsRef;

	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* MainBuildingMesh;

	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* WallMesh;

	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* WallMesh2;

	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* WallMesh3;

	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* WallMesh4;

	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* FirstFloorMesh;

	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* SecondFloorMesh;	
	
public:

	TArray<class AWindows*> GetWindowsArray() { return WindowsArray; }

	UChildActorComponent* GetWindowsChild() const { return WindowsChild; }

	UChildActorComponent* GetWindowsChild2() const { return WindowsChild2; }

	UChildActorComponent* GetWindowsChild3() const { return WindowsChild3; }

	UChildActorComponent* GetWindowsChild4() const { return WindowsChild4; }
};
