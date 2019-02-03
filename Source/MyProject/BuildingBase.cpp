// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildingBase.h"
#include "Windows.h"
#include "WindowEnemy.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/ChildActorComponent.h"
#include "Engine/GameEngine.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"


// Sets default values
ABuildingBase::ABuildingBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MainBuildingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Main Building"));
	RootComponent = MainBuildingMesh;

	WallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Wall"));
	WallMesh->SetupAttachment(MainBuildingMesh);

	WallMesh2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Wall 2"));
	WallMesh2->SetupAttachment(MainBuildingMesh);

	WallMesh3 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Wall 3"));
	WallMesh3->SetupAttachment(MainBuildingMesh);

	WallMesh4 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Wall 4"));
	WallMesh4->SetupAttachment(MainBuildingMesh);

	FirstFloorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("First Floor"));
	FirstFloorMesh->SetupAttachment(MainBuildingMesh);

	SecondFloorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Second Floor"));
	SecondFloorMesh->SetupAttachment(MainBuildingMesh);

	// Find windows class in content browser
	static ConstructorHelpers::FClassFinder<AWindows>
		WindowsBP(TEXT("Blueprint'/Game/Blueprints/WindowsBP.WindowsBP_C'"));
	if (WindowsBP.Succeeded() == true) {
		WindowsClass = (UClass*)WindowsBP.Class;
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Windows Not Found In BuildingBase!")));
	}

	// Find window enemy class in content browser
	static ConstructorHelpers::FClassFinder<AWindowEnemy>
		WindowEnemyBP(TEXT("Blueprint'/Game/Blueprints/WindowEnemyBP.WindowEnemyBP_C'"));
	if (WindowEnemyBP.Succeeded() == true) {
		WindowEnemyClass = (UClass*)WindowEnemyBP.Class;
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Window Enemy Not Found In BuildingBase!")));
	}

	WindowsChild = CreateDefaultSubobject<UChildActorComponent>(TEXT("Windows Child"));
	WindowsChild->SetChildActorClass(WindowsClass);
	WindowsChild->SetupAttachment(MainBuildingMesh);

	WindowsChild2 = CreateDefaultSubobject<UChildActorComponent>(TEXT("Windows Child 2"));
	WindowsChild2->SetChildActorClass(WindowsClass);
	WindowsChild2->SetupAttachment(MainBuildingMesh);

	WindowsChild3 = CreateDefaultSubobject<UChildActorComponent>(TEXT("Windows Child 3"));
	WindowsChild3->SetChildActorClass(WindowsClass);
	WindowsChild3->SetupAttachment(MainBuildingMesh);

	WindowsChild4 = CreateDefaultSubobject<UChildActorComponent>(TEXT("Windows Child 4"));
	WindowsChild4->SetChildActorClass(WindowsClass);
	WindowsChild4->SetupAttachment(MainBuildingMesh);
}

// Called when the game starts or when spawned
void ABuildingBase::BeginPlay()
{
	Super::BeginPlay();
	
	// Create the window actors and add them to array
	WindowsChild->CreateChildActor();
	EnemyHandler->SetWindowsActor(Cast<AWindows>(WindowsChild->GetChildActor()));

	WindowsChild2->CreateChildActor();
	EnemyHandler2->SetWindowsActor(Cast<AWindows>(WindowsChild2->GetChildActor()));

	WindowsChild3->CreateChildActor();
	EnemyHandler3->SetWindowsActor(Cast<AWindows>(WindowsChild3->GetChildActor()));

	WindowsChild4->CreateChildActor();
	EnemyHandler4->SetWindowsActor(Cast<AWindows>(WindowsChild4->GetChildActor()));

	EnemyHandler5->SetTerraceLoc(MainBuildingMesh->GetComponentLocation() + (MainBuildingMesh->GetForwardVector() * 580.0f) + (MainBuildingMesh->GetUpVector() * 100.0f));

	EnemyHandlerArray.Add(EnemyHandler);
	EnemyHandlerArray.Add(EnemyHandler2);
	EnemyHandlerArray.Add(EnemyHandler3);
	EnemyHandlerArray.Add(EnemyHandler4);
	EnemyHandlerArray.Add(EnemyHandler5);
}

// Called every frame
void ABuildingBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Spawn enemies just behind window or terrace
void ABuildingBase::SpawnEnemy(int32 Place)
{
	if (EnemyHandlerArray[Place]->GetEnemyActor() == nullptr) {
		
		EnemyHandlerArray[Place]->GetWindowsActor()->Open();
		FVector LocOffset;
		FRotator RotOffset;
		if (Place < 2) {
			LocOffset = (EnemyHandlerArray[Place]->GetWindowsActor()->GetActorRightVector() * -25.0f) + (EnemyHandlerArray[Place]->GetWindowsActor()->GetActorUpVector() * 10)
				+ (EnemyHandlerArray[Place]->GetWindowsActor()->GetActorForwardVector() * 10.0f);
			RotOffset = FRotator(0.0f, 90.0f, 0.0f);
		}
		else {
			LocOffset = (EnemyHandlerArray[Place]->GetWindowsActor()->GetActorRightVector() * -35.0f) + (EnemyHandlerArray[Place]->GetWindowsActor()->GetActorUpVector() * 10)
				+ (EnemyHandlerArray[Place]->GetWindowsActor()->GetActorForwardVector() * 10.0f);
			RotOffset = FRotator(0.0f, 90.0f, 0.0f);
		}
		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		AWindowEnemy* SpawnedEnemy = GetWorld()->SpawnActor<AWindowEnemy>(WindowEnemyClass, EnemyHandlerArray[Place]->GetWindowsActor()->GetActorLocation() + LocOffset, GetActorRotation() + RotOffset, ActorSpawnParams);

		SpawnedEnemy->SetWindowsPlace(Place);
		SpawnedEnemy->SetEnemyHandler(EnemyHandlerArray[Place]);
		EnemyHandlerArray[Place]->SetEnemyActor(SpawnedEnemy);
	}
}

