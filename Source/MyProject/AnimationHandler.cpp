// Fill out your copyright notice in the Description page of Project Settings.

#include "AnimationHandler.h"
#include "MyCharacter.h"
#include "Engine/GameEngine.h"
#include "Kismet/GameplayStatics.h"


void UAnimationHandler::UpdateAnimationProperties()
{
	AMyCharacter* CharacterRef = Cast<AMyCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	if (CharacterRef != NULL) {
		ForwardInput = CharacterRef->GetForwardInput();
		RightInput = CharacterRef->GetRightInput();
		bIdleAim = CharacterRef->GetZooming();
		WeaponInHand = CharacterRef->GetWInHand();
		bShootPistol = !CharacterRef->GetCanFirePistol();
		bShootRifle = !CharacterRef->GetCanFireRifle();
		bRifleSynch = CharacterRef->GetCanRifleSynch();
	}
	else {
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Char Not Found!")));
	}
}
