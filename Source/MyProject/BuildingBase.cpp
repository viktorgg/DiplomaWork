// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildingBase.h"
#include "Windows.h"
#include "WindowEnemy.h"
#include "MyProjectGameInstance.h"
#include "NationalBank.h"
#include "GeneralStore.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/ChildActorComponent.h"
#include "Engine/GameEngine.h"
#include "Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Sound/SoundCue.h"
#include "Runtime/Core/Public/Templates/SharedPointer.h"
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

	// Find the Window cue in content browser by reference
	static ConstructorHelpers::FObjectFinder<USoundCue>
		CueAsset(TEXT("SoundCue'/Game/Assets/Sound/Window_Cue.Window_Cue'"));
	if (CueAsset.Succeeded() == true) {
		WindowSqueak = CueAsset.Object;
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Window Cue Not Found In BuildingBase!")));
	}

	WindowsChild = CreateDefaultSubobject<UChildActorComponent>(TEXT("Windows Child"));
	WindowsChild->SetupAttachment(MainBuildingMesh);
	
	WindowsChild2 = CreateDefaultSubobject<UChildActorComponent>(TEXT("Windows Child 2"));
	WindowsChild2->SetupAttachment(MainBuildingMesh);

	WindowsChild3 = CreateDefaultSubobject<UChildActorComponent>(TEXT("Windows Child 3"));
	WindowsChild3->SetupAttachment(MainBuildingMesh);

	WindowsChild4 = CreateDefaultSubobject<UChildActorComponent>(TEXT("Windows Child 4"));
	WindowsChild4->SetupAttachment(MainBuildingMesh);
}

// Called when the game starts or when spawned
void ABuildingBase::BeginPlay()
{
	Super::BeginPlay();

}

void ABuildingBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// Set the class of child actor
	WindowsChild->SetChildActorClass(WindowsClass);
	WindowsChild2->SetChildActorClass(WindowsClass);
	WindowsChild3->SetChildActorClass(WindowsClass);
	WindowsChild4->SetChildActorClass(WindowsClass);

	// Create the window actors and add them to array
	WindowsChild->CreateChildActor();
	WindowsChild2->CreateChildActor();
	WindowsChild3->CreateChildActor();
	WindowsChild4->CreateChildActor();

	// Create structures for 5 possible enemies
	FEnemyHandler* EnemyHandler = new FEnemyHandler();
	FEnemyHandler* EnemyHandler2 = new FEnemyHandler();
	FEnemyHandler* EnemyHandler3 = new FEnemyHandler();
	FEnemyHandler* EnemyHandler4 = new FEnemyHandler();
	FEnemyHandler* EnemyHandler5 = new FEnemyHandler();

	EnemyHandler->WindowsActor = Cast<AWindows>(WindowsChild->GetChildActor());
	EnemyHandler2->WindowsActor = Cast<AWindows>(WindowsChild2->GetChildActor());
	EnemyHandler3->WindowsActor = Cast<AWindows>(WindowsChild3->GetChildActor());
	EnemyHandler4->WindowsActor = Cast<AWindows>(WindowsChild4->GetChildActor());
	EnemyHandler5->TerraceLoc = MainBuildingMesh->GetComponentLocation() + (MainBuildingMesh->GetForwardVector() * 580.0f) + (MainBuildingMesh->GetUpVector() * 100.0f);

	// Add the structs to array
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
	if (EnemyHandlerArray[Place] && !EnemyHandlerArray[Place]->EnemyActor && EnemyHandlerArray[Place]->WindowsActor->GetIsClosed()) {
		
		EnemyHandlerArray[Place]->WindowsActor->Open();

		float VolumeControl = Cast<UMyProjectGameInstance>(GetWorld()->GetGameInstance())->VolumeControl;
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), WindowSqueak, EnemyHandlerArray[Place]->WindowsActor->GetActorLocation(), VolumeControl, WindowSqueak->GetPitchMultiplier());

		FVector LocOffset;
		FRotator RotOffset;

		// Configure rotational offset depending on building
		if (Cast<ANationalBank>(this)) {
			RotOffset = FRotator(0.f, 90.f, 0.f);
		}
		else if (Cast<AGeneralStore>(this)) {
			RotOffset = FRotator(0.f, -90.f, 0.f);
		}

		// Configure spawn location and rotation depending on floor
		if (Place < 2) {
			LocOffset = (EnemyHandlerArray[Place]->WindowsActor->GetActorRightVector() * -25.0f) + (EnemyHandlerArray[Place]->WindowsActor->GetActorUpVector() * 10)
				+ (EnemyHandlerArray[Place]->WindowsActor->GetActorForwardVector() * 10.0f);
		}
		else {
			LocOffset = (EnemyHandlerArray[Place]->WindowsActor->GetActorRightVector() * -35.0f) + (EnemyHandlerArray[Place]->WindowsActor->GetActorUpVector() * 10)
				+ (EnemyHandlerArray[Place]->WindowsActor->GetActorForwardVector() * 10.0f);
		}
		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		AWindowEnemy* SpawnedEnemy = GetWorld()->SpawnActor<AWindowEnemy>(WindowEnemyClass, EnemyHandlerArray[Place]->WindowsActor->GetActorLocation() + LocOffset, GetActorRotation() + RotOffset, ActorSpawnParams);

		SpawnedEnemy->SetWindowsPlace(Place);
		SpawnedEnemy->SetEntryRotation(GetActorRotation() + RotOffset);
		SpawnedEnemy->SetBuildingActor(this);
		SpawnedEnemy->SetEnemyHandler(EnemyHandlerArray[Place]);
		SpawnedEnemy->SetWindowSqueak(WindowSqueak);
		EnemyHandlerArray[Place]->EnemyActor = SpawnedEnemy;
	}
}

