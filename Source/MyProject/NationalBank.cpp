// Fill out your copyright notice in the Description page of Project Settings.

#include "NationalBank.h"
#include "Windows.h"
#include "WindowEnemy.h"
#include "MyCharacter.h"
#include "Engine/GameEngine.h"
#include "Engine/World.h"


// Sets default values
ANationalBank::ANationalBank() {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void ANationalBank::BeginPlay()
{
	Super::BeginPlay();
	
}

void ANationalBank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


