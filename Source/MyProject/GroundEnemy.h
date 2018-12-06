// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterBase.h"
#include "GroundEnemy.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API AGroundEnemy : public ACharacterBase
{
	GENERATED_BODY()
	
public:

	AGroundEnemy();

	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;


private:

	UPROPERTY(EditAnywhere)
		float DistanceToWalk;

	TSubclassOf<class ARevolver> PistolClass;

	class AMyCharacter* MainCharacterActor;

	virtual void MoveForward(float Input);

	virtual void Fire();

	virtual void ResetPistolFire();

	virtual void RotateToCharacter();
	
	float GetDistanceToMain();

	void Rotate(float Direction);

	float LineTrace();

};
