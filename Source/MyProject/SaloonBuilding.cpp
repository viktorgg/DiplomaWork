// Fill out your copyright notice in the Description page of Project Settings.

#include "SaloonBuilding.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"


// Sets default values
ASaloonBuilding::ASaloonBuilding()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MainBuildingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Main Building Mesh"));
	RootComponent = MainBuildingMesh;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	BoxCollision->SetupAttachment(MainBuildingMesh);

	BoxCollision2 = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision 2"));
	BoxCollision2->SetupAttachment(MainBuildingMesh);

	BoxCollision3 = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision 3"));
	BoxCollision3->SetupAttachment(MainBuildingMesh);

	BoxCollision4 = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision 4"));
	BoxCollision4->SetupAttachment(MainBuildingMesh);

	BoxCollision5 = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision 5"));
	BoxCollision5->SetupAttachment(MainBuildingMesh);

	BoxCollision6 = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision 6"));
	BoxCollision6->SetupAttachment(MainBuildingMesh);
}

// Called when the game starts or when spawned
void ASaloonBuilding::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASaloonBuilding::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

