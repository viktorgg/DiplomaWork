// Fill out your copyright notice in the Description page of Project Settings.

#include "AnimationHandler.h"
#include "MyCharacter.h"
#include "Engine/GameEngine.h"
#include "Kismet/GameplayStatics.h"


void UAnimationHandler::UpdateAnimationProperties()
{
	AMyCharacter* Character = Cast<AMyCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	if (Character) {
		ForwardInput = Character->ForwardInput;
		RightInput = Character->RightInput;
		bIdleAim = Character->bCanZoom;
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Char Not Found!")));
	}
}
