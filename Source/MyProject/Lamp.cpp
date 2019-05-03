// Fill out your copyright notice in the Description page of Project Settings.

#include "Lamp.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/PointLightComponent.h"
#include "Engine/World.h"
#include "Engine/GameEngine.h"


// Sets default values
ALamp::ALamp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bIfTurnOn = false;
	MaxIntensity = 2750.f;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	LampMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Lamp Mesh"));
	LampMesh->SetupAttachment(Root);

	PointLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("Point Light"));
	PointLight->SetupAttachment(LampMesh);
	PointLight->SetIntensity(0.f);
}

// Called when the game starts or when spawned
void ALamp::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALamp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIfTurnOn && !FMath::IsNearlyEqual(PointLight->Intensity, MaxIntensity, 1.f)) {
		TurnOnLight();
	}
	else if(!bIfTurnOn && !FMath::IsNearlyEqual(PointLight->Intensity, 0.f, 1.f)){
		TurnOffLight();
	}

	// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("%f"), PointLight->Intensity));
}

void ALamp::TurnOnLight()
{
	PointLight->SetIntensity(FMath::FInterpConstantTo(PointLight->Intensity, MaxIntensity, GetWorld()->GetDeltaSeconds(), 100.f));
}

void ALamp::TurnOffLight()
{
	PointLight->SetIntensity(FMath::FInterpConstantTo(PointLight->Intensity, 0.f, GetWorld()->GetDeltaSeconds(), 100.f));
}

