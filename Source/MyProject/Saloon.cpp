// Fill out your copyright notice in the Description page of Project Settings.

#include "Saloon.h"


// Sets default values
ASaloon::ASaloon() {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void ASaloon::BeginPlay()
{
	Super::BeginPlay();
}

void ASaloon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASaloon::SpawnEnemy(int32 Place)
{
}
