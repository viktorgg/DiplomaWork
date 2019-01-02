// Fill out your copyright notice in the Description page of Project Settings.

#include "SaloonBuilding.h"
#include "BuildingBase.h"
#include "WindowEnemy.h"
#include "SaloonGroundEnemy.h"
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

	static ConstructorHelpers::FClassFinder<ASaloonGroundEnemy>
		SaloonGroundEnemyBP(TEXT("Blueprint'/Game/Blueprints/SaloonGroundEnemyBP.SaloonGroundEnemyBP_C'"));
	if (SaloonGroundEnemyBP.Succeeded() == true) {
		SaloonGroundEnemyClass = (UClass*)SaloonGroundEnemyBP.Class;
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Ground Enemy Not Found!")));
	}
}

// Called when the game starts or when spawned
void ASaloonBuilding::BeginPlay()
{
	Super::BeginPlay();

	//Inverted Directions
	FSEnemyHandler SEnemyHandler;
	SEnemyHandler.SetInLoc(FVector(MainBuildingMesh->GetComponentLocation() + (MainBuildingMesh->GetForwardVector() * 180.0f) 
		+ (MainBuildingMesh->GetRightVector() * 365.0f) + (MainBuildingMesh->GetUpVector() * -181.0f)));

	FSEnemyHandler SEnemyHandler2;
	SEnemyHandler2.SetOutLoc(FVector(MainBuildingMesh->GetComponentLocation() + (MainBuildingMesh->GetForwardVector() * 4.0f)
		+ (MainBuildingMesh->GetRightVector() * -470.0f) + (MainBuildingMesh->GetUpVector() * 137.0f)));

	FSEnemyHandler SEnemyHandler3;
	SEnemyHandler3.SetDoorLoc(FVector(MainBuildingMesh->GetComponentLocation() + (MainBuildingMesh->GetRightVector() * -100.0f) + (MainBuildingMesh->GetUpVector() * -157.0f)));

	SEnemyHandlerArray.Add(SEnemyHandler);
	SEnemyHandlerArray.Add(SEnemyHandler2);
	SEnemyHandlerArray.Add(SEnemyHandler3);
}

// Called every frame
void ASaloonBuilding::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SpawnEnemy(1);

	for (int32 i = 0; i < 2; i++) {
		if (SEnemyHandlerArray[i].GetEnemyActor() != NULL) {
			if (SEnemyHandlerArray[i].GetEnemyActor()->GetHealth() <= 0) {
				SEnemyHandlerArray[i].SetEnemyActor(NULL);
			}
		}
	}
}

void ASaloonBuilding::SpawnEnemy(int32 Place)
{
	if (SEnemyHandlerArray[Place].GetEnemyActor() == NULL) {

		if (Place == 0) {
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
			AWindowEnemy* SpawnedEnemy = GetWorld()->SpawnActor<AWindowEnemy>(WindowEnemyClass, SEnemyHandlerArray[Place].GetInLoc(), GetActorRotation(), ActorSpawnParams);

			SEnemyHandlerArray[Place].SetEnemyActor(SpawnedEnemy);
		}
		else if (Place == 1) {
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
			AWindowEnemy* SpawnedEnemy = GetWorld()->SpawnActor<AWindowEnemy>(WindowEnemyClass, SEnemyHandlerArray[Place].GetOutLoc(), GetActorRotation(), ActorSpawnParams);

			SEnemyHandlerArray[Place].SetEnemyActor(SpawnedEnemy);
		}
		else if (Place == 2) {
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
			ASaloonGroundEnemy* SpawnedEnemy = GetWorld()->SpawnActor<ASaloonGroundEnemy>(SaloonGroundEnemyClass, SEnemyHandlerArray[Place].GetDoorLoc(), GetActorRotation() + FRotator(0.0f, -90.0f, 0.0f), ActorSpawnParams);

			SEnemyHandlerArray[Place].SetEnemyActor(SpawnedEnemy);
		}
	}
}

