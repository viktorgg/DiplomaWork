// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterBase.h"
#include "WindowEnemy.generated.h"

/**
 * 
 */
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

	TSubclassOf<class ARifle> RifleClass;

	class AMyCharacter* MainCharacterActor;

	class AWindows* WindowsActor;

	int32 WindowsPlace;

	virtual void Fire();

	virtual void ResetRifleFire();

	virtual void RotateToCharacter();

public:

	void SetWindowsPlace(int32 NewWindowsPlace) { WindowsPlace = NewWindowsPlace; }

	class AWindows* GetWindowsActor() const { return WindowsActor; }
	void SetWindowsActor(class AWindows* NewWindowsActor) { WindowsActor = NewWindowsActor; }
	
};
