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

	UPROPERTY()
		UClass* PistolClass;

	//bool bShouldMove;
	bool bIsRotating;

	float RotSpeed;

	UPROPERTY(EditAnywhere)
		float DistanceToWalk;

	virtual void MoveForward(float Input) override;

	virtual void Fire() override;

	virtual void ResetPistolFire() override;

	virtual void DestroyChar() override;

	void RotateToCharacter();

	void StationaryRotation();

	float GetDistanceToMain();

	void Rotate(float Direction);

	float LineTrace();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void DestroyAfterTime();

	UFUNCTION(BlueprintCallable)
		bool GetIsRotating() const { return bIsRotating; }

	void SetDistanceToWalk(float DistanceToWalk) { this->DistanceToWalk = DistanceToWalk; }

	float GetRotSpeed() { return RotSpeed; }
	void SetRotSpeed(float RotSpeed) { this->RotSpeed = RotSpeed; }
};
