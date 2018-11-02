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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float ForwardInput;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float RightInput;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bIdleAim;
	
	UFUNCTION(BlueprintCallable)
		void UpdateAnimationProperties();
};
