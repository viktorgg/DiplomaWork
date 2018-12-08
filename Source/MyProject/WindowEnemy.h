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

	virtual void Fire();

	virtual void ResetRifleFire();

	virtual void RotateToCharacter();
	
};
