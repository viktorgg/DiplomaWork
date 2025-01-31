// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterBase.h"
#include "MyCharacter.generated.h"


UENUM()
enum WeaponInHand { None, Pistol, Rifle };

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

	virtual void DestroyAfterTime() override;

	void ZoomedKills(float Distance);

	// References to buildings arrays in order to deallocate memory
	TArray<FEnemyHandler*> BankEnemyHandlerArray;
	TArray<FEnemyHandler*> StoreEnemyHandlerArray;
	TArray<FEnemyHandler*> HotelEnemyHandlerArray;
	TArray<FEnemyHandler*> SaloonEnemyHandlerArray;
	TArray<FEnemyHandler*> Saloon2EnemyHandlerArray;

private:

	int32 KilledEnemies;

	float ZoomedCharSpeed;
	float NotZoomedCharSpeed;

	float SlowMoCapacity;

	UPROPERTY(EditAnywhere)
		float LookSpeed;
	float LookUpperLimit;
	float LookLowerLimit;

	float HealthRegenSpeed;

	bool bZooming;

	bool bSlowMo;
	UPROPERTY()
		class USoundCue* SlowMoWoosh;
	UPROPERTY()
		class USoundCue* UnholsterWeapon;

	WeaponInHand WInHand;

	bool bZoomedKills;
	// Timer to handle camera zoom on eliminating enemies
	FTimerHandle ZoomedKillHandle;

	int32 CurrPistolMagazine;
	UPROPERTY(EditAnywhere)
		int32 PistolMagazineLimit;

	int32 CurrRifleMagazine;
	UPROPERTY(EditAnywhere)
		int32 RifleMagazineLimit;

	UPROPERTY(VisibleAnywhere)
		class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere)
		class UCameraComponent* Camera;

	virtual void MoveForward(float Input) override;

	virtual void MoveRight(float Input) override;

	virtual void Fire() override;

	virtual void ResetPistolFire() override;

	virtual void ResetRifleFire() override;

	virtual void DestroyChar() override;

	void LookSide(float Input);

	void LookUp(float Input);

	void CameraZoom();

	void CameraOutZoom();

	void LerpPlayerToCamera(float Speed);

	void ResetRifleAnim();

	void ChangeToPistol();

	void ChangeToRifle();

	void EnterSlowMo();

	void ResetZoomedKills();

public:

	UFUNCTION(BlueprintCallable)
		int32 GetKilledEnemies() const { return KilledEnemies; }
	void SetKilledEnemies(int32 KilledEnemies) { this->KilledEnemies = KilledEnemies; }

	bool GetZooming() const { return bZooming; }

	UFUNCTION(BlueprintCallable)
		WeaponInHand GetWInHand() const { return WInHand; }

	UFUNCTION(BlueprintCallable)
		float GetLookSpeed() const { return LookSpeed; }
	UFUNCTION(BlueprintCallable)
		void SetLookSpeed(float NewLookSpeed) { LookSpeed = NewLookSpeed; }

	UCameraComponent* GetCamera() const { return Camera; }

	UFUNCTION(BlueprintCallable)
		int32 GetCurrPistolMagazine() const { return CurrPistolMagazine; }
	void SetCurrPistolMagazine(int32 CurrPistolMagazine) { this->CurrPistolMagazine = CurrPistolMagazine; }

	int32 GetPistolMagazineLimit() const { return PistolMagazineLimit; }

	UFUNCTION(BlueprintCallable)
		int32 GetCurrRifleMagazine() const { return CurrRifleMagazine; }
	void SetCurrRifleMagazine(int32 CurrRifleMagazine) { this->CurrRifleMagazine = CurrRifleMagazine; }

	int32 GetRifleMagazineLimit() const { return RifleMagazineLimit; }

	UFUNCTION(BlueprintCallable)
		float GetSlowMoCapacity() const { return SlowMoCapacity; }
	void AddSlowMoCapacity(float Amount) { if (SlowMoCapacity < 10.0f) { SlowMoCapacity += Amount; } }

	UFUNCTION(BlueprintCallable)
		bool GetSlowMo() const { return bSlowMo; }

};