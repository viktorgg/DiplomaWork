// Fill out your copyright notice in the Description page of Project Settings.

#include "GameHUD.h"
#include "Engine/GameEngine.h"
#include "Engine/Canvas.h"
#include "Engine/Texture2D.h"
#include "TextureResource.h"
#include "CanvasItem.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"


AGameHUD::AGameHUD()
{
	static ConstructorHelpers::FObjectFinder<UTexture2D> 
		Crosshair(TEXT("/Game/Assets/HUD/FirstPersonCrosshair"));
	if (Crosshair.Object != NULL) {
		CrosshairTex = Crosshair.Object;
	}
	
}

