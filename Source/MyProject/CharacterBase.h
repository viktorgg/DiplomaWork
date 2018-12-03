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

	bool bCanRifleAnim;
	FTimerHandle RifleAnimHandle;

	class ARevolver* PistolActor;
	class ARifle* RifleActor;

	virtual void MoveForward(float Input) PURE_VIRTUAL(ACharacterBase::MoveForward, );

	virtual void MoveRight(float Input) PURE_VIRTUAL(ACharacterBase::MoveRight, );

	virtual void Fire() PURE_VIRTUAL(ACharacterBase::Fire, );

	virtual void ResetPistolFire() PURE_VIRTUAL(ACharacterBase::ResetPistolFire, );

	virtual void ResetRifleFire() PURE_VIRTUAL(ACharacterBase::ResetRifleFire, );

	virtual void ResetRifleSynch() PURE_VIRTUAL(ACharacterBase::ResetRifleSynch, );


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	int32 GetHealth() const { return Health; }
	void SetHealth(int32 NewHealth) { Health = NewHealth; }

	float GetCharacterSpeed() const { return CharacterSpeed; }
	void SetCharacterSpeed(float NewCharacterSpeed) { CharacterSpeed = NewCharacterSpeed; }

	UFUNCTION(BlueprintCallable)
		float GetForwardInput() const { return ForwardInput; }
	void SetForwardInput(float Input) { ForwardInput = Input; }

	float GetRightInput() const { return RightInput; }
	void SetRightInput(float Input) { RightInput = Input; }

	bool GetHavePistol() const { return bHavePistol; }
	void SetHavePistol(bool bNewHavePistol) { bHavePistol = bNewHavePistol; }

	bool GetHaveRifle() const { return bHaveRifle; }
	void SetHaveRifle(bool bNewHaveRifle) { bHaveRifle = bNewHaveRifle; }

	UFUNCTION(BlueprintCallable)
		bool GetCanFirePistol() const { return bCanFirePistol; }
	void SetCanFirePistol(bool bNewCanFirePistol) { bCanFirePistol = bNewCanFirePistol; }

	float GetPistolFireRate() const { return PistolFireRate; }
	void SetPistolFireRate(float NewPistolFireRate) { PistolFireRate = NewPistolFireRate; }

	UFUNCTION(BlueprintCallable)
		bool GetCanFireRifle() const { return bCanFireRifle; }
	void SetCanFireRifle(bool bNewCanFireRifle) { bCanFireRifle = bNewCanFireRifle; }

	bool GetCanRifleAnim() const { return bCanRifleAnim; }
	void SetCanRifleAnim(bool bNewCanRifleAnim) { bCanRifleAnim = bNewCanRifleAnim; }

	float GetRifleFireRate() const { return RifleFireRate; }
	void SetRifleFireRate(float NewRifleFireRate) { RifleFireRate = NewRifleFireRate; }

	FTimerHandle &GetPistolFireRateHandle() { return PistolFireRateHandle; }

	FTimerHandle &GetRifleFireRateHandle() { return RifleFireRateHandle; }

	FTimerHandle &GetRifleAnimHandle() { return RifleAnimHandle; }

	ARevolver* GetPistolActor() const { return PistolActor; }
	void SetPistolActor(ARevolver* NewPistolActor) { PistolActor = NewPistolActor; }

	ARifle* GetRifleActor() const { return RifleActor; }
	void SetRifleActor(ARifle* NewRifleActor) { RifleActor = NewRifleActor; }


};

