// Fill out your copyright notice in the Description page of Project Settings.

#include "SaloonBuilding.h"
#include "BuildingBase.h"
#include "WindowEnemy.h"
#include "GroundEnemy.h"
#include "MyCharacter.h"
#include "LevelHandler.h"
#include "SaloonGroundEnemy.h"
#include "MyProjectGameInstance.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ChildActorComponent.h"
#include "Components/BoxComponent.h"
#include "Engine/GameEngine.h"
#include "Kismet/GameplayStatics.h"
#include "Runtime/Engine/Public/EngineUtils.h"
#include "Runtime/Engine/Public/TimerManager.h"
#include "Runtime/Core/Public/Templates/SharedPointer.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"


// Sets default values
ASaloonBuilding::ASaloonBuilding()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	SaloonIndex = 0;

	bHasEntered = false;

	MainBuildingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Main Building Mesh"));
	RootComponent = MainBuildingMesh;

	DoorsChild = CreateDefaultSubobject<UChildActorComponent>(TEXT("Windows Child"));
	DoorsChild->SetupAttachment(MainBuildingMesh);

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	BoxCollision->SetupAttachment(MainBuildingMesh);
	
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

	// Calls OnEnterBox when something enters sphere
	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &ASaloonBuilding::OnEnterBox); 
	// Calls OnLeaveBox when something leaves sphere
	BoxCollision->OnComponentEndOverlap.AddDynamic(this, &ASaloonBuilding::OnLeaveBox);

	// Set the array in MyCharacter for memory deallocation
	for (TActorIterator<AMyCharacter> ActorItr(GetWorld()); ActorItr; ++ActorItr) {
		if (ActorItr) {
			if (SaloonIndex == 1) {
				ActorItr->SaloonEnemyHandlerArray = SEnemyHandlerArray;
			}
			else if (SaloonIndex == 2) {
				ActorItr->Saloon2EnemyHandlerArray = SEnemyHandlerArray;
			}
		}
	}
}

void ASaloonBuilding::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	// Create structures for 3 possible enemies
	FEnemyHandler* SEnemyHandler = new FEnemyHandler();
	FEnemyHandler* SEnemyHandler2 = new FEnemyHandler();
	FEnemyHandler* SEnemyHandler3 = new FEnemyHandler();

	//Inverted Directions, forward vector = right vector, right vector = forward vector
	SEnemyHandler->InLoc = FVector(MainBuildingMesh->GetComponentLocation() + (MainBuildingMesh->GetForwardVector() * 180.0f)
		+ (MainBuildingMesh->GetRightVector() * 365.0f) + (MainBuildingMesh->GetUpVector() * -181.0f));

	SEnemyHandler2->OutLoc = FVector(MainBuildingMesh->GetComponentLocation() + (MainBuildingMesh->GetForwardVector() * 4.0f)
		+ (MainBuildingMesh->GetRightVector() * -510.0f) + (MainBuildingMesh->GetUpVector() * 137.0f));

	SEnemyHandler3->DoorLoc = FVector(MainBuildingMesh->GetComponentLocation() + (MainBuildingMesh->GetRightVector() * -100.0f) + (MainBuildingMesh->GetUpVector() * -200.0f));

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
	if (SEnemyHandlerArray[Place] && !SEnemyHandlerArray[Place]->EnemyActor) {

		if (Place == 0) {		// Spawns behind bar
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
			AWindowEnemy* SpawnedEnemy = GetWorld()->SpawnActor<AWindowEnemy>(WindowEnemyClass, SEnemyHandlerArray[Place]->InLoc, GetActorRotation(), ActorSpawnParams);

			SEnemyHandlerArray[Place]->EnemyActor = SpawnedEnemy;
			SpawnedEnemy->SetEnemyHandler(SEnemyHandlerArray[Place]);
		}
		else if (Place == 1) {		// Spawns on terrace
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
			AWindowEnemy* SpawnedEnemy = GetWorld()->SpawnActor<AWindowEnemy>(WindowEnemyClass, SEnemyHandlerArray[Place]->OutLoc, GetActorRotation(), ActorSpawnParams);

			SEnemyHandlerArray[Place]->EnemyActor = SpawnedEnemy;
			SpawnedEnemy->SetEnemyHandler(SEnemyHandlerArray[Place]);
		}
		else if (Place == 2) {		// Spawns behind door
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
			ASaloonGroundEnemy* SpawnedEnemy = GetWorld()->SpawnActor<ASaloonGroundEnemy>(SaloonGroundEnemyClass, SEnemyHandlerArray[Place]->DoorLoc, GetActorRotation() + FRotator(0.0f, -90.0f, 0.0f), ActorSpawnParams);

			SEnemyHandlerArray[Place]->EnemyActor = SpawnedEnemy;
			SpawnedEnemy->SetEnemyHandler(SEnemyHandlerArray[Place]);
		}
	}
}

void ASaloonBuilding::OnEnterBox(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor && Cast<AMyCharacter>(OtherActor)) {
		// bHasEntered = true;

		GetWorldTimerManager().SetTimer(GenemiesDistanceHandle, this, &ASaloonBuilding::SetGEnemiesInDistance, 2, true, 1);
		// Spawn extra GEnemy when difficulty isn't easy
		for (TActorIterator<ALevelHandler> ActorItr(GetWorld()); ActorItr; ++ActorItr) {
			if (ActorItr) {
				if (Cast<UMyProjectGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))->DifficultyAmount != Easy) {
					ActorItr->SpawnGroundEnemy(1);
				}
			}
		}
	}
}

void ASaloonBuilding::OnLeaveBox(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && Cast<AMyCharacter>(OtherActor)) {
		// Stops timer
		GetWorldTimerManager().ClearTimer(GenemiesDistanceHandle);
		// bHasEntered = false;

		SetGEnemiesOutDistance();
	}
}

void ASaloonBuilding::SetGEnemiesInDistance()
{
	// Get all GEnemies from level
	for (TActorIterator<AGroundEnemy> ActorItr(GetWorld()); ActorItr; ++ActorItr) {
		if (ActorItr) {
			ActorItr->SetDistanceToWalk(FMath::RandRange(200.f, 300.f));
			if (ActorItr->GetRotSpeed() != 5.f) {
				ActorItr->SetRotSpeed(ActorItr->GetRotSpeed() * 2.f);
			}
		}
	}
}

void ASaloonBuilding::SetGEnemiesOutDistance()
{
	// Get all GEnemies from level
	for (TActorIterator<AGroundEnemy> ActorItr(GetWorld()); ActorItr; ++ActorItr) {
		if (ActorItr) {
			ActorItr->SetDistanceToWalk(FMath::RandRange(450.f, 750.f));
			ActorItr->SetRotSpeed(ActorItr->GetRotSpeed() / 2.f);
		}
	}
}



