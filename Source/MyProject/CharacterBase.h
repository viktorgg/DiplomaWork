// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterBase.generated.h"

UCLASS()
class MYPROJECT_API ACharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACharacterBase();

	UPROPERTY(EditAnywhere)
		int32 Health;
	UPROPERTY(EditAnywhere)
		float CharacterSpeed;

	float ForwardInput;
	float RightInput;

	bool bHavePistol;
	bool bHaveRifle;

	bool bCanFirePistol;
	float RevolverFireRate;
	FTimerHandle PistolFireRate;

	FTimerHandle NoZoomFire;

	class ARevolver* RevolverRef;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	virtual void MoveForward(float Input) PURE_VIRTUAL(ACharacterBase::MoveForward, );

	virtual void MoveRight(float Input) PURE_VIRTUAL(ACharacterBase::MoveRight, );

	virtual void Fire() PURE_VIRTUAL(ACharacterBase::Fire, );

	virtual void ResetPistolFire() PURE_VIRTUAL(ACharacterBase::ResetPistolFire, );

	virtual void FireAfterDelay() PURE_VIRTUAL(ACharacterBase::FireAfterDelay(), );

};

