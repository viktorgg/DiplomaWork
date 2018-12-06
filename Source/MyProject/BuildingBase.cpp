// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildingBase.h"
#include "Windows.h"
#include "WindowEnemy.h"
#include "Components/StaticMeshComponent.h"
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

	static ConstructorHelpers::FClassFinder<AWindows>
		WindowsBP(TEXT("Blueprint'/Game/Blueprints/WindowsBP.WindowsBP_C'"));
	if (WindowsBP.Succeeded() == true) {
		WindowsClass = (UClass*)WindowsBP.Class;
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Windows Not Found!")));
	}

	static ConstructorHelpers::FClassFinder<AWindowEnemy>
		WindowEnemyBP(TEXT("Blueprint'/Game/Blueprints/WindowEnemyBP.WindowEnemyBP_C'"));
	if (WindowEnemyBP.Succeeded() == true) {
		WindowEnemyClass = (UClass*)WindowEnemyBP.Class;
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Window Enemy Not Found!")));
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
	
	WindowsChild->CreateChildActor();
	WindowsChild2->CreateChildActor();
	WindowsChild3->CreateChildActor();
	WindowsChild4->CreateChildActor();

	WindowsArray.Add(Cast<AWindows>(WindowsChild->GetChildActor()));
	WindowsArray.Add(Cast<AWindows>(WindowsChild2->GetChildActor()));
	WindowsArray.Add(Cast<AWindows>(WindowsChild3->GetChildActor()));
	WindowsArray.Add(Cast<AWindows>(WindowsChild4->GetChildActor()));
}

// Called every frame
void ABuildingBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

