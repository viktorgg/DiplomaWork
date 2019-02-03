// Fill out your copyright notice in the Description page of Project Settings.

#include "CharacterBase.h"
#include "Revolver.h"
#include "Rifle.h"
#include "MyCharacter.h"
#include "GroundEnemy.h"
#include "WindowEnemy.h"
#include "Windows.h"
#include "Components/CapsuleComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Runtime/Engine/Public/EngineUtils.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Animation/AnimationAsset.h"
#include "Runtime/Engine/Public/TimerManager.h"
#include "Engine/GameEngine.h"


// Sets default values
ACharacterBase::ACharacterBase()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Health = 0.0f;
	CharacterSpeed = 0.0f;

	ForwardInput = 0.0f;
	RightInput = 0.0f;

	bHavePistol = false;
	bHaveRifle = false;

	bCanFirePistol = true;
	PistolFireRate = 0.0f;

	bCanFireRifle = true;
	RifleFireRate = 0.0f;

	bCanRifleAnim = true;

	PistolActor = nullptr;
	RifleActor = nullptr;
	MainCharacterActor = nullptr;

	RootComponent = GetCapsuleComponent();

	GetArrowComponent()->SetupAttachment(RootComponent);

	GetMesh()->SetupAttachment(RootComponent);

	// Find the Main Char DeathAnimation asset in content browser by reference
	static ConstructorHelpers::FObjectFinder<UAnimSequence>
		AnimAsset(TEXT("AnimSequence'/Game/Assets/Animations/MainChar/Death.Death'"));
	if (AnimAsset.Succeeded() == true) {
		MainCharDeathAnim = AnimAsset.Object;
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Animation1 Not Found In CharBase!")));
	}

	// Find the Enemy DeathAnimation asset in content browser by reference
	static ConstructorHelpers::FObjectFinder<UAnimSequence>
		AnimAsset2(TEXT("AnimSequence'/Game/Assets/Animations/EnemyChar/Shoulder_Hit_And_Fall.Shoulder_Hit_And_Fall'"));
	if (AnimAsset2.Succeeded() == true) {
		EnemyDeathAnim = AnimAsset2.Object;
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Animation2 Not Found In CharBase!")));
	}

	// Find the Enemy DeathAnimation2 asset in content browser by reference
	static ConstructorHelpers::FObjectFinder<UAnimSequence>
		AnimAsset3(TEXT("AnimSequence'/Game/Assets/Animations/EnemyChar/Dying.Dying'"));
	if (AnimAsset3.Succeeded() == true) {
		EnemyDeathAnim2 = AnimAsset3.Object;
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Animation3 Not Found In CharBase!")));
	}
}

// Called when the game starts or when spawned
void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();

	// Find the player controlled character; Enemies' target
	for (TActorIterator<AMyCharacter> ActorItr(GetWorld()); ActorItr; ++ActorItr) {
		if (ActorItr) {
			MainCharacterActor = *ActorItr;
		}
	}
}

void ACharacterBase::PlayMainDeathAnim()
{
	GetMesh()->PlayAnimation(MainCharDeathAnim, false);
}

void ACharacterBase::PlayEnemyDeathAnim()
{
	if (Cast<AWindowEnemy>(this) != NULL) {
		GetMesh()->PlayAnimation(EnemyDeathAnim, false);
	}
	else {
		int32 Anim = FMath::RandRange(1, 2);

		if (Anim == 1) {
			GetMesh()->PlayAnimation(EnemyDeathAnim, false);
		}
		else {
			GetMesh()->PlayAnimation(EnemyDeathAnim2, false);
		}
	}
}

// Called every frame
void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Find the rotation the enemy needs to be to face player controlled character
FRotator ACharacterBase::LookAtChar()
{
	FRotator LookAtChar = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), MainCharacterActor->GetActorLocation());
	return LookAtChar;
}

