// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SaloonBuilding.generated.h"


USTRUCT()
struct FSEnemyHandler
{
	GENERATED_USTRUCT_BODY()

private:

	class ACharacterBase* EnemyActor;

	FVector InLoc;
	FVector DoorLoc;
	FVector OutLoc;

public:

	ACharacterBase* GetEnemyActor() const { return EnemyActor; }
	void SetEnemyActor(ACharacterBase* EnemyActor) { this->EnemyActor = EnemyActor; }

	FVector GetInLoc() const { return InLoc; }
	void SetInLoc(FVector InLoc) { this->InLoc = InLoc; }

	FVector GetDoorLoc() const { return DoorLoc; }
	void SetDoorLoc(FVector DoorLoc) { this->DoorLoc = DoorLoc; }

	FVector GetOutLoc() const { return OutLoc; }
	void SetOutLoc(FVector OutLoc) { this->OutLoc = OutLoc; }

	FSEnemyHandler() {
		EnemyActor = NULL;
	}
};


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

	TArray<FSEnemyHandler> SEnemyHandlerArray;

	void SpawnEnemy(int32 Place);

private:
	
	TSubclassOf<class AWindowEnemy> WindowEnemyClass;
	TSubclassOf<class ASaloonGroundEnemy> SaloonGroundEnemyClass;

	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* MainBuildingMesh;

	UPROPERTY(VisibleAnywhere)
		class UChildActorComponent* DoorsChild;

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
