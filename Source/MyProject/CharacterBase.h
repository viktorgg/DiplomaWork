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


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


private:

	UPROPERTY(EditAnywhere)
		int32 Health;
	UPROPERTY(EditAnywhere)
		float CharacterSpeed;

	float ForwardInput;
	float RightInput;

	bool bHavePistol;
	bool bHaveRifle;

	bool bCanFirePistol;
	float PistolFireRate;
	FTimerHandle PistolFireRateHandle;

	bool bCanFireRifle;
	float RifleFireRate;
	FTimerHandle RifleFireRateHandle;

	FTimerHandle NoZoomFireHandle;

	class ARevolver* PistolRef;
	class ARifle* RifleRef;

	virtual void MoveForward(float Input) PURE_VIRTUAL(ACharacterBase::MoveForward, );

	virtual void MoveRight(float Input) PURE_VIRTUAL(ACharacterBase::MoveRight, );

	virtual void Fire() PURE_VIRTUAL(ACharacterBase::Fire, );

	virtual void ResetPistolFire() PURE_VIRTUAL(ACharacterBase::ResetPistolFire, );

	virtual void ResetRifleFire() PURE_VIRTUAL(ACharacterBase::ResetRifleFire, );


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	int32 GetHealth() const { return Health; }
	void SetHealth(int32 Input) { Health = Input; }

	float GetCharacterSpeed() const { return CharacterSpeed; }
	void SetCharacterSpeed(float Input) { CharacterSpeed = Input; }

	UFUNCTION(BlueprintCallable)
		float GetForwardInput() const { return ForwardInput; }
	void SetForwardInput(float Input) { ForwardInput = Input; }

	float GetRightInput() const { return RightInput; }
	void SetRightInput(float Input) { RightInput = Input; }

	bool GetHavePistol() const { return bHavePistol; }
	void SetHavePistol(bool Input) { bHavePistol = Input; }

	bool GetHaveRifle() const { return bHaveRifle; }
	void SetHaveRifle(bool Input) { bHaveRifle = Input; }

	UFUNCTION(BlueprintCallable)
		bool GetCanFirePistol() const { return bCanFirePistol; }
	void SetCanFirePistol(bool Input) { bCanFirePistol = Input; }

	float GetPistolFireRate() const { return PistolFireRate; }
	void SetPistolFireRate(float Input) { PistolFireRate = Input; }

	bool GetCanFireRifle() const { return bCanFireRifle; }
	void SetCanFireRifle(bool Input) { bCanFireRifle = Input; }

	float GetRifleFireRate() const { return RifleFireRate; }
	void SetRifleFireRate(float Input) { RifleFireRate = Input; }

	FTimerHandle &GetPistolFireRateHandle() { return PistolFireRateHandle; }

	FTimerHandle &GetRifleFireRateHandle() { return RifleFireRateHandle; }

	FTimerHandle &GetNoZoomFireHandle() { return NoZoomFireHandle; }

	ARevolver* GetPistolRef() const { return PistolRef; }
	void SetPistolRef(ARevolver* Input) { PistolRef = Input; }

	ARifle* GetRifleRef() const { return RifleRef; }
	void SetRifleRef(ARifle* Input) { RifleRef = Input; }


};

