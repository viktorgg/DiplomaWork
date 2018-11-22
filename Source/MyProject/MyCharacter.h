// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterBase.h"
#include "MyCharacter.generated.h"

UCLASS()
class MYPROJECT_API AMyCharacter : public ACharacterBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


private:

	float CharacterNormalSpeed;

	UPROPERTY(EditAnywhere)
		float LookSpeed;
	float LookUpperLimit;
	float LookLowerLimit;

	bool bZooming;
	bool bOutZooming;

	enum WeaponInHand { None = 0, Pistol = 1, Rifle = 2 };
	WeaponInHand WInHand;

	UPROPERTY(VisibleAnywhere)
		class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere)
		class UCameraComponent* Camera;

	virtual void MoveForward(float Input);

	virtual void MoveRight(float Input);

	virtual void Fire();

	virtual void ResetPistolFire();

	virtual void ResetRifleFire();

	virtual void FireAfterDelay();

	void LookSide(float Input);

	void LookUp(float Input);

	void CameraZoom();

	void CameraOutZoom();

	void LerpPlayerToCamera(float Speed);

	void ChangeToPistol();

	void ChangeToRifle();

public:

	bool GetZooming() const { return bZooming; }

	WeaponInHand GetWInHand() const { return WInHand; }

	UCameraComponent* GetCamera() const { return Camera; }
};
