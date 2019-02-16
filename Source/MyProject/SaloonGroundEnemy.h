// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GroundEnemy.h"
#include "SaloonGroundEnemy.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API ASaloonGroundEnemy : public AGroundEnemy
{
	GENERATED_BODY()
	

public:

	ASaloonGroundEnemy();
	

protected:

	virtual void BeginPlay() override;

private:

	bool bIsKicking;
	FTimerHandle KickingHandle;

	FTimerHandle HalfwayKickHandle;
	class USoundCue* DoorKick;

	void ResetKicking();

	void PlaySound();

	virtual void MoveForward(float Input) override;
	
public:

	UFUNCTION(BlueprintCallable)
		bool GetIsKicking() const { return bIsKicking; }
};
