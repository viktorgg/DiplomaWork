// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BuildingBase.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterBase.generated.h"


UCLASS(Abstract)
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
		float Health;

	float ForwardInput;
	float RightInput;

	bool bHavePistol;
	bool bHaveRifle;

	// Timer to handle enemy characters fire delay after spawn 
	bool bCanFire;
	FTimerHandle FireDelayHandle;

	// Timer to handle pistol fire rate
	bool bCanFirePistol;
	float PistolFireRate;
	FTimerHandle PistolFireRateHandle;

	// Timer to handle rifle fire rate
	bool bCanFireRifle;
	float RifleFireRate;
	FTimerHandle RifleFireRateHandle;

	// Timer to handle rifle animation transitions (visual purposes)
	bool bCanRifleAnim;
	FTimerHandle RifleAnimHandle;

	// Timer to handle destruction of character
	FTimerHandle DestroyHandle;

	// Timer to handle hit reactions
	bool bIsHit;
	bool bCanHit;
	FTimerHandle HitDelay;
	FTimerHandle HitRegularity;

	UPROPERTY()
		class ARevolver* PistolActor;
	UPROPERTY()
		class ARifle* RifleActor;
	UPROPERTY()
		class AMyCharacter* MainCharacterActor;
	UPROPERTY()
		class ALevelHandler* LevelHandlerActor;

	UPROPERTY()
		UAnimSequence* MainCharDeathAnim;
	UPROPERTY()
		UAnimSequence* EnemyDeathAnim;
	UPROPERTY()
		UAnimSequence* EnemyDeathAnim2;

	UPROPERTY()
		class USoundCue* WindowSqueak;

	FEnemyHandler* EnemyHandler;

	void ResetHitDelay();

	void ResetHitRegularity();

	void ResetFireDelay();

	virtual void MoveForward(float Input) PURE_VIRTUAL(ACharacterBase::MoveForward, );

	virtual void MoveRight(float Input) PURE_VIRTUAL(ACharacterBase::MoveRight, );

	virtual void Fire() PURE_VIRTUAL(ACharacterBase::Fire, );

	virtual void RotateToMainChar() PURE_VIRTUAL(ACharacterBase::RotateToMainChar, );

	virtual void ResetPistolFire() PURE_VIRTUAL(ACharacterBase::ResetPistolFire, );

	virtual void ResetRifleFire() PURE_VIRTUAL(ACharacterBase::ResetRifleFire, );

	virtual void ResetRifleSynch() PURE_VIRTUAL(ACharacterBase::ResetRifleSynch, );

	virtual void DestroyAfterTime() PURE_VIRTUAL(ACharacterBase::DestroyAfterTime, );

	virtual void DestroyChar() PURE_VIRTUAL(ACharacterBase::DestroyChar, );

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FRotator LookAtChar();

	void PlayMainDeathAnim();

	void PlayEnemyDeathAnim();

	UFUNCTION(BlueprintCallable)
		float GetHealth() const { return Health; }
	void SetHealth(float Health) { this->Health = Health; }

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

	UFUNCTION(BlueprintCallable)
		AMyCharacter* GetMainCharacterActor() const { return MainCharacterActor; }

	UAnimSequence* GetMainDeathAnim() const { return MainCharDeathAnim; }

	UAnimSequence* GetEnemyDeathAnim() const { return EnemyDeathAnim; }

	UFUNCTION(BlueprintCallable)
		bool GetIsHit() const { return bIsHit; }
	void SetIsHit(bool bIsHit);

	void SetEnemyHandler(FEnemyHandler* EnemyHandler) { this->EnemyHandler = EnemyHandler; }

	void SetWindowSqueak(USoundCue* WindowSqueak) { this->WindowSqueak = WindowSqueak; }
};