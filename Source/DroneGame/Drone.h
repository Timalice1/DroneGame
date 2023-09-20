// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Drone.generated.h"

UCLASS()
class DRONEGAME_API ADrone : public APawn
{
	GENERATED_BODY()

public:
	ADrone();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;

//Components
protected:
	UPROPERTY(EditAnywhere, Category = "Drone");
	USphereComponent* Collider;

	UFloatingPawnMovement* PawnMovement;

	UPROPERTY(EditAnywhere, Category = "Drone")
	USkeletalMeshComponent* Mesh;

//Actions
protected:
	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	void MoveUp(float AxisValue);

	void StartShooting();
	void StopShooting();
	void Fire();

//Drone properties
protected:
	UPROPERTY(EditAnywhere, meta = (ClampMin = 0))
	float MaxSpeed = 1200.0f;

	UPROPERTY(EditAnywhere, meta = (ClampMin = 0))
	float Acceleration = 800.0f;

	UPROPERTY(EditAnywhere, meta = (ClampMin = 0))
	float Deceleration = 1200.0f;

	UPROPERTY(EditAnywhere, meta = (ClampMin = 0))
	float FireRate;

private: 
	FTimerHandle FireRateTimer;
};
