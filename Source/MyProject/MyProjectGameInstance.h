// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BuildingBase.h"
#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MyProjectGameInstance.generated.h"

/**
 * 
 */

UENUM()
enum GameDifficulty { Easy, Medium, Hard };


UCLASS()
class MYPROJECT_API UMyProjectGameInstance : public UGameInstance
{
	GENERATED_BODY()
	

public:

	UMyProjectGameInstance();

	UPROPERTY(BlueprintReadWrite)
		float MouseSensitivity;

	UPROPERTY(BlueprintReadWrite)
		float VolumeControl;

	UPROPERTY(BlueprintReadWrite)
		TEnumAsByte<GameDifficulty> DifficultyAmount;
};
