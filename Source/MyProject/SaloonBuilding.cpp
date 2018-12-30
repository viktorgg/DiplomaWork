// Fill out your copyright notice in the Description page of Project Settings.

#include "SaloonBuilding.h"
#include "BuildingBase.h"
#include "WindowEnemy.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ChildActorComponent.h"
#include "Components/BoxComponent.h"
#include "Engine/GameEngine.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"


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

	DoorsChild = CreateDefaultSubobject<UChildActorComponent>(TEXT("Windows Child"));
	DoorsChild->SetupAttachment(MainBuildingMesh);

	static ConstructorHelpers::FClassFinder<AWindowEnemy>
		WindowEnemyBP(TEXT("Blueprint'/Game/Blueprints/WindowEnemyBP.WindowEnemyBP_C'"));
	if (WindowEnemyBP.Succeeded() == true) {
		WindowEnemyClass = (UClass*)WindowEnemyBP.Class;
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Window Enemy Not Found!")));
	}
	
}

// Called when the game starts or when spawned
void ASaloonBuilding::BeginPlay()
{
	Super::BeginPlay();

	FSEnemyHandler SEnemyHandler;
	SEnemyHandler.SetInLoc(FVector(MainBuildingMesh->GetComponentLocation() + (MainBuildingMesh->GetForwardVector() * 180.0f) 
		+ (MainBuildingMesh->GetRightVector() * 365.0f) + (MainBuildingMesh->GetUpVector() * -181.0f)));

	FSEnemyHandler SEnemyHandler2;
	SEnemyHandler2.SetOutLoc(FVector(MainBuildingMesh->GetComponentLocation() + (MainBuildingMesh->GetForwardVector() * 4.0f)
		+ (MainBuildingMesh->GetRightVector() * -470.0f) + (MainBuildingMesh->GetUpVector() * 137.0f)));

	SEnemyHandlerArray.Add(SEnemyHandler);
	SEnemyHandlerArray.Add(SEnemyHandler2);

}

// Called every frame
void ASaloonBuilding::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASaloonBuilding::SpawnEnemy(int32 Place)
{
	if (Place == 0) {
		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		AWindowEnemy* SpawnedEnemy = GetWorld()->SpawnActor<AWindowEnemy>(WindowEnemyClass, SEnemyHandlerArray[Place].GetInLoc(), GetActorRotation(), ActorSpawnParams);

		SEnemyHandlerArray[Place].SetWindowEnemyActor(SpawnedEnemy);
	}
	else if(Place == 1){
		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		AWindowEnemy* SpawnedEnemy = GetWorld()->SpawnActor<AWindowEnemy>(WindowEnemyClass, SEnemyHandlerArray[Place].GetOutLoc(), GetActorRotation(), ActorSpawnParams);

		SEnemyHandlerArray[Place].SetWindowEnemyActor(SpawnedEnemy);
	}
}

