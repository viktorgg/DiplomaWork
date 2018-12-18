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

	FTimerHandle DestroyHandle;

	class ARevolver* PistolActor;
	class ARifle* RifleActor;
	class AMyCharacter* MainCharacterActor;

	UAnimSequence* DeathAnim;

	virtual void MoveForward(float Input) PURE_VIRTUAL(ACharacterBase::MoveForward, );

	virtual void MoveRight(float Input) PURE_VIRTUAL(ACharacterBase::MoveRight, );

	virtual void Fire() PURE_VIRTUAL(ACharacterBase::Fire, );

	virtual void RotateToMainChar() PURE_VIRTUAL(ACharacterBase::RotateToMainChar, );

	virtual void ResetPistolFire() PURE_VIRTUAL(ACharacterBase::ResetPistolFire, );

	virtual void ResetRifleFire() PURE_VIRTUAL(ACharacterBase::ResetRifleFire, );

	virtual void ResetRifleSynch() PURE_VIRTUAL(ACharacterBase::ResetRifleSynch, );

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	FRotator LookAtChar();

	void PlayDeathAnim();

	void DestroyAfterTime();

	void DestroyChar();

	int32 GetHealth() const { return Health; }
	void SetHealth(int32 Health) { this->Health = Health; }

	float GetCharacterSpeed() const { return CharacterSpeed; }
	void SetCharacterSpeed(float CharacterSpeed) { this->CharacterSpeed = CharacterSpeed; }

	UFUNCTION(BlueprintCallable)
		float GetForwardInput() const { return ForwardInput; }
	void SetForwardInput(float Input) { ForwardInput = Input; }

	float GetRightInput() const { return RightInput; }
	void SetRightInput(float Input) { RightInput = Input; }

	bool GetHavePistol() const { return bHavePistol; }
	void SetHavePistol(bool bHavePistol) { this->bHavePistol = bHavePistol; }

	bool GetHaveRifle() const { return bHaveRifle; }
	void SetHaveRifle(bool bHaveRifle) { this->bHaveRifle = bHaveRifle; }

	UFUNCTION(BlueprintCallable)
		bool GetCanFirePistol() const { return bCanFirePistol; }
	void SetCanFirePistol(bool bCanFirePistol) { this->bCanFirePistol = bCanFirePistol; }

	float GetPistolFireRate() { return PistolFireRate; }
	void SetPistolFireRate(float PistolFireRate) { this->PistolFireRate = PistolFireRate; }

	UFUNCTION(BlueprintCallable)
		bool GetCanFireRifle() const { return bCanFireRifle; }
	void SetCanFireRifle(bool bCanFireRifle) { this->bCanFireRifle = bCanFireRifle; }

	bool GetCanRifleAnim() const { return bCanRifleAnim; }
	void SetCanRifleAnim(bool bCanRifleAnim) { this->bCanRifleAnim = bCanRifleAnim; }

	float GetRifleFireRate() const { return RifleFireRate; }
	void SetRifleFireRate(float RifleFireRate) { this->RifleFireRate = RifleFireRate; }

	ARevolver* GetPistolActor() const { return PistolActor; }
	void SetPistolActor(ARevolver* PistolActor) { this->PistolActor = PistolActor; }

	ARifle* GetRifleActor() const { return RifleActor; }
	void SetRifleActor(ARifle* RifleActor) { this->RifleActor = RifleActor; }

	AMyCharacter* GetMainCharacterActor() const { return MainCharacterActor; }

	UAnimSequence* GetDeathAnim() const { return DeathAnim; }

};

