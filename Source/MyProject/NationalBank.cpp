// Fill out your copyright notice in the Description page of Project Settings.

#include "NationalBank.h"
#include "Windows.h"
#include "Engine/GameEngine.h"



// Sets default values
ANationalBank::ANationalBank() {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


}

void ANationalBank::BeginPlay()
{
	Super::BeginPlay();

	for (int32 i = 0; i < GetWindowsArray().Num(); i++) {
		GetWindowsArray()[i]->Open();
	}
}

void ANationalBank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
