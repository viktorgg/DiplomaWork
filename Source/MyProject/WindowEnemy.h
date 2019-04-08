// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BuildingBase.h"
#include "CoreMinimal.h"
#include "CharacterBase.h"
#include "WindowEnemy.generated.h"


UCLASS()
class MYPROJECT_API AWindowEnemy : public ACharacterBase
{
	GENERATED_BODY()


public:

	AWindowEnemy();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;


private:

	UPROPERTY()
		UClass* RifleClass;

	// Need to know the window it stands behind in order to adjust rotation
	int32 WindowsPlace;

	// Need to know the rotation when spawned in order to limit it properly
	FRotator EntryRotation;

	UPROPERTY()
		class ABuildingBase* BuildingActor;

	UPROPERTY()
		class USoundCue* Scream;

	virtual void Fire() override;

	virtual void ResetRifleFire() override;

	virtual void DestroyChar() override;

	void RotateToCharacter();

public:

	virtual void DestroyAfterTime() override;

	void SetWindowsPlace(int32 WindowsPlace) { this->WindowsPlace = WindowsPlace; }
	
	void SetBuildingActor(ABuildingBase* BuildingActor) { this->BuildingActor = BuildingActor; }

	void SetEntryRotation(FRotator EntryRotation) { this->EntryRotation = EntryRotation; }
};
