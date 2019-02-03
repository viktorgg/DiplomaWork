// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "AnimationHandler.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UAnimationHandler : public UAnimInstance
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float ForwardInput;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float RightInput;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool bIdleAim;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int32 WeaponInHand;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool bShootPistol;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool bShootRifle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool bRifleAnim;
	
	UFUNCTION(BlueprintCallable)
		void UpdateAnimationProperties();

};
