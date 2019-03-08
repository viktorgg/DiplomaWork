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

	DoorsChild = CreateDefaultSubobject<UChildActorComponent>(TEXT("Windows Child"));
	DoorsChild->SetupAttachment(MainBuildingMesh);

	// Find window enemy class in content browser
	static ConstructorHelpers::FClassFinder<AWindowEnemy>
		WindowEnemyBP(TEXT("Blueprint'/Game/Blueprints/WindowEnemyBP.WindowEnemyBP_C'"));
	if (WindowEnemyBP.Succeeded() == true) {
		WindowEnemyClass = (UClass*)WindowEnemyBP.Class;
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Window Enemy Not Found In Saloon!")));
	}

	// Find saloon ground enemy class in content browser
	static ConstructorHelpers::FClassFinder<ASaloonGroundEnemy>
		SaloonGroundEnemyBP(TEXT("Blueprint'/Game/Blueprints/SaloonGroundEnemyBP.SaloonGroundEnemyBP_C'"));
	if (SaloonGroundEnemyBP.Succeeded() == true) {
		SaloonGroundEnemyClass = (UClass*)SaloonGroundEnemyBP.Class;
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Ground Enemy Not Found In Saloon!")));
	}

}

// Called when the game starts or when spawned
void ASaloonBuilding::BeginPlay()
{
	Super::BeginPlay();

	// Create structures for 3 possible enemies
	FEnemyHandler* SEnemyHandler = new FEnemyHandler();
	FEnemyHandler* SEnemyHandler2 = new FEnemyHandler();
	FEnemyHandler* SEnemyHandler3 = new FEnemyHandler();

	//Inverted Directions, forward vector = right vector, right vector = forward vector
	SEnemyHandler->SetInLoc(FVector(MainBuildingMesh->GetComponentLocation() + (MainBuildingMesh->GetForwardVector() * 180.0f) 
		+ (MainBuildingMesh->GetRightVector() * 365.0f) + (MainBuildingMesh->GetUpVector() * -181.0f)));

	SEnemyHandler2->SetOutLoc(FVector(MainBuildingMesh->GetComponentLocation() + (MainBuildingMesh->GetForwardVector() * 4.0f)
		+ (MainBuildingMesh->GetRightVector() * -470.0f) + (MainBuildingMesh->GetUpVector() * 137.0f)));

	SEnemyHandler3->SetDoorLoc(FVector(MainBuildingMesh->GetComponentLocation() + (MainBuildingMesh->GetRightVector() * -100.0f) + (MainBuildingMesh->GetUpVector() * -157.0f)));


	// Add the structs to array
	SEnemyHandlerArray.Add(SEnemyHandler);
	SEnemyHandlerArray.Add(SEnemyHandler2);
	SEnemyHandlerArray.Add(SEnemyHandler3);
}

// Called every frame
void ASaloonBuilding::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASaloonBuilding::SpawnEnemy(int32 Place)
{
	if (SEnemyHandlerArray[Place]->GetEnemyActor() == nullptr) {

		if (Place == 0) {		// Spawns behind bar
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
			AWindowEnemy* SpawnedEnemy = GetWorld()->SpawnActor<AWindowEnemy>(WindowEnemyClass, SEnemyHandlerArray[Place]->GetInLoc(), GetActorRotation(), ActorSpawnParams);

			SEnemyHandlerArray[Place]->SetEnemyActor(SpawnedEnemy);
			SpawnedEnemy->SetEnemyHandler(SEnemyHandlerArray[Place]);
		}
		else if (Place == 1) {		// Spawns on terrace
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
			AWindowEnemy* SpawnedEnemy = GetWorld()->SpawnActor<AWindowEnemy>(WindowEnemyClass, SEnemyHandlerArray[Place]->GetOutLoc(), GetActorRotation(), ActorSpawnParams);

			SEnemyHandlerArray[Place]->SetEnemyActor(SpawnedEnemy);
			SpawnedEnemy->SetEnemyHandler(SEnemyHandlerArray[Place]);
		}
		else if (Place == 2) {		// Spawns behind door
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
			ASaloonGroundEnemy* SpawnedEnemy = GetWorld()->SpawnActor<ASaloonGroundEnemy>(SaloonGroundEnemyClass, SEnemyHandlerArray[Place]->GetDoorLoc(), GetActorRotation() + FRotator(0.0f, -90.0f, 0.0f), ActorSpawnParams);

			SEnemyHandlerArray[Place]->SetEnemyActor(SpawnedEnemy);
			SpawnedEnemy->SetEnemyHandler(SEnemyHandlerArray[Place]);
		}
	}
}

