// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SaloonBuilding.h"
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

	UClass* PistolClass;

	bool bIsRotating;

	UPROPERTY(EditAnywhere)
		float DistanceToWalk;

	virtual void MoveForward(float Input) override;

	virtual void Fire() override;

	virtual void ResetPistolFire() override;

	virtual void DestroyChar() override;

	void RotateToCharacter();

	float GetDistanceToMain();

	void Rotate(float Direction);

	float LineTrace();


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void DestroyAfterTime();

	UFUNCTION(BlueprintCallable)
		bool GetIsRotating() const { return bIsRotating; }
};
