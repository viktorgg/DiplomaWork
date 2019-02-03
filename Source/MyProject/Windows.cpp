// Fill out your copyright notice in the Description page of Project Settings.

#include "Windows.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Engine/World.h"
#include "Engine/GameEngine.h"
#include "Runtime/Engine/Public/TimerManager.h"

// Sets default values
AWindows::AWindows()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bOpening = false;
	bClosing = false;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	WindowLMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WindowL Mesh"));
	WindowLMesh->SetupAttachment(Root);

	WindowRMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WindowR Mesh"));
	WindowRMesh->SetupAttachment(Root);
}

// Called when the game starts or when spawned
void AWindows::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AWindows::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bOpening == true) {
		Open();
	}
	if (bClosing == true) {
		Close();
	}
	// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("%d"), GetIfOpened()));
}

void AWindows::Open()
{
	if (FMath::IsNearlyEqual(WindowRMesh->GetRelativeTransform().GetRotation().Rotator().Yaw, 110.0f, 2.0f) == false) {

		bOpening = true;

		FRotator WindowRCurrRot = WindowRMesh->GetRelativeTransform().GetRotation().Rotator();
		FRotator WindowRNewRot = FRotator(0.0f, 110.0f, 0.0f);

		FRotator WindowLCurrRot = WindowLMesh->GetRelativeTransform().GetRotation().Rotator();
		FRotator WindowLNewRot = FRotator(0.0f, -110.0f, 0.0f);

		WindowRMesh->SetRelativeRotation(FMath::RInterpConstantTo(WindowRCurrRot, WindowRNewRot, GetWorld()->DeltaTimeSeconds, 150.0f));
		WindowLMesh->SetRelativeRotation(FMath::RInterpConstantTo(WindowLCurrRot, WindowLNewRot, GetWorld()->DeltaTimeSeconds, 150.0f));
	}
	else {
		bOpening = false;
	}
}

void AWindows::Close()
{
	if (FMath::IsNearlyEqual(WindowRMesh->GetRelativeTransform().GetRotation().Rotator().Yaw, -90.0f, 2.0f) == false) {

		bClosing = true;

		FRotator WindowRCurrRot = WindowRMesh->GetRelativeTransform().GetRotation().Rotator();
		FRotator WindowRNewRot = FRotator(0.0f, -90.0f, 0.0f);

		FRotator WindowLCurrRot = WindowLMesh->GetRelativeTransform().GetRotation().Rotator();
		FRotator WindowLNewRot = FRotator(0.0f, 90.0f, 0.0f);

		WindowRMesh->SetRelativeRotation(FMath::RInterpConstantTo(WindowRCurrRot, WindowRNewRot, GetWorld()->DeltaTimeSeconds, 150.0f));
		WindowLMesh->SetRelativeRotation(FMath::RInterpConstantTo(WindowLCurrRot, WindowLNewRot, GetWorld()->DeltaTimeSeconds, 150.0f));
	}
	else {
		bClosing = false;
	}
}

bool AWindows::GetIfOpened() const
{
	if (FMath::IsNearlyEqual(WindowRMesh->GetRelativeTransform().GetRotation().Rotator().Yaw, -90.0f, 2.0f) == true) {

		return false; 
	}
	else if (FMath::IsNearlyEqual(WindowRMesh->GetRelativeTransform().GetRotation().Rotator().Yaw, 110.0f, 2.0f) == true) {

		return true;
	}
	else {
		return NULL;
	}
}


