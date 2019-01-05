// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BuildingBase.generated.h"


// Structure to hold information about enemy and it's window
USTRUCT()
struct FEnemyHandler
{
	GENERATED_USTRUCT_BODY()

private:

	class AWindows* WindowsActor;

	class AWindowEnemy* WindowEnemyActor;

	FVector TerraceLoc;

public:

	AWindows* GetWindowsActor() const { return WindowsActor; }
	void SetWindowsActor(AWindows* WindowsActor) { this->WindowsActor = WindowsActor; }

	AWindowEnemy* GetWindowEnemyActor() const { return WindowEnemyActor; }
	void SetWindowEnemyActor(AWindowEnemy* WindowEnemyActor) { this->WindowEnemyActor = WindowEnemyActor; }

	FVector GetTerraceLoc() const { return TerraceLoc; }
	void SetTerraceLoc(FVector TerraceLoc) { this->TerraceLoc = TerraceLoc; }

	FEnemyHandler() {
		WindowsActor = NULL;
		WindowEnemyActor = NULL;
	}
};

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

	UClass* WindowsClass;

	UClass* WindowEnemyClass;

	UPROPERTY(VisibleAnywhere)
		class UChildActorComponent* WindowsChild;

	UPROPERTY(VisibleAnywhere)
		class UChildActorComponent* WindowsChild2;

	UPROPERTY(VisibleAnywhere)
		class UChildActorComponent* WindowsChild3;

	UPROPERTY(VisibleAnywhere)
		class UChildActorComponent* WindowsChild4;

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
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void SpawnEnemy(int32 Place);

	// Array to hold structure of every enemy
	TArray<FEnemyHandler> EnemyHandlerArray;

	UClass* GetWindowEnemyClass() const { return WindowEnemyClass; }

	UChildActorComponent* GetWindowsChild() const { return WindowsChild; }

	UChildActorComponent* GetWindowsChild2() const { return WindowsChild2; }

	UChildActorComponent* GetWindowsChild3() const { return WindowsChild3; }

	UChildActorComponent* GetWindowsChild4() const { return WindowsChild4; }
};
