// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyCharacter.generated.h"

UCLASS()
class MYPROJECT_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UPROPERTY(EditAnywhere)
		float PlayerSpeed;

	UPROPERTY(EditAnywhere)
		float LookSpeed;

	UPROPERTY(EditAnywhere)
		float LookUpperLimit;

	UPROPERTY(EditAnywhere)
		float LookLowerLimit;

	UPROPERTY(VisibleAnywhere)
		class UCapsuleComponent* CapsuleCollision;

	UPROPERTY(VisibleAnywhere)
		class USkeletalMeshComponent* PlayerMesh;

	UPROPERTY(VisibleAnywhere)
		class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere)
		class UCameraComponent* Camera;
	
	UFUNCTION()
		void MoveForward(float input);

	UFUNCTION()
		void MoveRight(float input);

	UFUNCTION()
		void LookSide(float input);

	UFUNCTION()
		void LookUp(float input);

	UFUNCTION()
		void LerpToRot();
};
