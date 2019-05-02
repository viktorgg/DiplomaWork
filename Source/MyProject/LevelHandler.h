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

	UPROPERTY()
		TArray<AActor*> SaloonArray;

	bool bEntered;

	enum GEnemySpawn { Start, Barn, End };
	GEnemySpawn GESpawn;

	enum WEnemyBuilding { Bank, Hotel, Store, Saloon, Saloon2 };
	WEnemyBuilding WEBuilding;

	FVector GEnemyStart = FVector(2910.f, -722.f, 207.f);
	FVector GEnemyBarn = FVector(1104.f, 2335.f, 207.f);
	FVector GEnemyEnd = FVector(3610.f, 3633.f, 207.f);

	// Timer to handle day and night cycle
	FTimerHandle DayDurationHandle;

	// Timers to handle WEnemies spawn consistency
	FTimerHandle WEnemyHandle;
	FTimerHandle HotelTerraceHandle;
	FTimerHandle SaloonOutHandle;
	FTimerHandle SaloonOutHandle2;

	FTimerDelegate HotelTerraceDel;
	FTimerDelegate SaloonOutDel;
	FTimerDelegate SaloonOutDel2;

	UPROPERTY(VisibleAnywhere)
		class USceneComponent* Root;

	UPROPERTY(VisibleAnywhere)
		class UBoxComponent* BoxCollision;

	UPROPERTY(VisibleAnywhere)
		class UBoxComponent* BoxCollisionBarn;

	UPROPERTY(VisibleAnywhere)
		class UBoxComponent* BoxCollisionEnd;

	UPROPERTY()
		class AMyCharacter* MainCharacter;

	UPROPERTY()
		class AGroundEnemy* GroundEnemy;

	UPROPERTY()
		UClass* GroundEnemyClass;
	UPROPERTY()
		UClass* SaloonBuildingClass;

	UPROPERTY()
	class ASaloonBuilding* SaloonBuildingActor;
		UPROPERTY()
		class ASaloonBuilding* SaloonBuildingActor2;
	UPROPERTY()
		class ANationalBank* NationalBankActor;
	UPROPERTY()
		class AHotel* HotelActor;
	UPROPERTY()
		class AGeneralStore* GeneralStoreActor;

	bool IfEnemyInSight(FVector Loc);

	UFUNCTION()
		void SpawnBankEnemy(int32 Place);

	UFUNCTION()
		void SpawnHotelEnemy(int32 Place);

	UFUNCTION()
		void SpawnStoreEnemy(int32 Place);

	UFUNCTION()
		void SpawnSaloonEnemy(int32 Place);

	UFUNCTION()
		void SpawnSaloonEnemy2(int32 Place);

	UFUNCTION()
		virtual void OnEnterBox(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void SetLampState();

public:

	UFUNCTION()
		void SpawnGroundEnemy(int32 Place);

	ANationalBank* GetNationalBankActor() const { return NationalBankActor; }

	AHotel* GetHotelActor() const { return HotelActor; }

	AGeneralStore* GetGeneralStoreActor() const { return GeneralStoreActor; }

	ASaloonBuilding* GetSaloonBuildingActor() const { return SaloonBuildingActor; }

	ASaloonBuilding* GetSaloonBuildingActor2() const { return SaloonBuildingActor2; }
};
