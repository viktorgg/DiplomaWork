// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Windows.generated.h"

UCLASS()
class MYPROJECT_API AWindows : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWindows();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Open();

	void Close();

	bool GetIsClosed();
	
private:

	bool bOpening;
	bool bClosing;

	UPROPERTY(VisibleAnywhere)
		class USceneComponent* Root;

	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* WindowLMesh;

	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* WindowRMesh;

};
