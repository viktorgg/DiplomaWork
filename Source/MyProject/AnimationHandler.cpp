// Fill out your copyright notice in the Description page of Project Settings.

#include "AnimationHandler.h"
#include "MyCharacter.h"
#include "Engine/GameEngine.h"
#include "Kismet/GameplayStatics.h"


void UAnimationHandler::UpdateAnimationProperties()
{
	AMyCharacter* CharacterRef = Cast<AMyCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	if (CharacterRef != NULL) {
		ForwardInput = CharacterRef->ForwardInput;
		RightInput = CharacterRef->RightInput;
		bIdleAim = CharacterRef->bZooming;
		WeaponInHand = CharacterRef->WInHand;
		bShootPistol = !CharacterRef->bCanFirePistol;
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Char Not Found!")));
	}
}
