// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelHandler.generated.h"

UCLASS()
class MYPROJECT_API ALevelHandler : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALevelHandler();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	void GEnemyHandler();

	void WEnemyHandler();

private:

	bool bEntered;

	enum GEnemySpawn { Start, Barn, End };
	GEnemySpawn GESpawn;

	FVector GEnemyStart = FVector(2910.f, -722.f, 205.f);
	FVector GEnemyBarn = FVector(1104.f, 2335.f, 205.f);
	FVector GEnemyEnd = FVector(3555.f, 2898.f, 205.f);

	FTimerDelegate BankEnemyDel;
	FTimerHandle BankEnemyHandle;

	FTimerHandle GEnemyHandle;

	UPROPERTY(VisibleAnywhere)
		class USceneComponent* Root;

	UPROPERTY(VisibleAnywhere)
		class UBoxComponent* BoxCollision;

	UPROPERTY(VisibleAnywhere)
		class UBoxComponent* BoxCollisionBarn;

	UPROPERTY(VisibleAnywhere)
		class UBoxComponent* BoxCollisionEnd;

	class AMyCharacter* MainCharacter;

	class AGroundEnemy* GroundEnemy;

	UClass* GroundEnemyClass;

	class ASaloonBuilding* SaloonBuildingActor;

	class ANationalBank* NationalBankActor;

	class AHotel* HotelActor;

	class AGeneralStore* GeneralStoreActor;

	// One time use
	void SpawnGEnemy();

	bool IfEnemyInSight(FVector Loc, FRotator Rot);

	UFUNCTION()
		void SpawnGroundEnemy(int32 Place);

	UFUNCTION()
		void SpawnBankEnemy(int32 Place);

	UFUNCTION()
		virtual void OnEnterBox(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


};
