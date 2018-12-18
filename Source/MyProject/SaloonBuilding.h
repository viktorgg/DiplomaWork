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

	class AWindowEnemy* WindowEnemyActor;

	FVector InLoc;
	FVector OutLoc;

public:

	AWindowEnemy* GetWindowEnemyActor() const { return WindowEnemyActor; }
	void SetWindowEnemyActor(AWindowEnemy* WindowEnemyActor) { this->WindowEnemyActor = WindowEnemyActor; }

	FVector GetInLoc() const { return InLoc; }
	void SetInLoc(FVector InLoc) { this->InLoc = InLoc; }

	FVector GetOutLoc() const { return OutLoc; }
	void SetOutLoc(FVector OutLoc) { this->OutLoc = OutLoc; }

	FSEnemyHandler() {
		WindowEnemyActor = NULL;
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

private:

	TSubclassOf<class AWindowEnemy> WindowEnemyClass;

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

	void SpawnEnemy(int32 Place);
	
};
