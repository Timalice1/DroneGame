// Fill out your copyright notice in the Description page of Project Settings.


#include "Drone.h"

ADrone::ADrone()
{
	PrimaryActorTick.bCanEverTick = true;
	bUseControllerRotationYaw = true;

	Collider = CreateDefaultSubobject<USphereComponent>(TEXT("Collider"));
	Collider->SetCollisionProfileName(TEXT("Pawn"));
	RootComponent = Collider;

	PawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("PawnMovement"));
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("DroneMesh"));
}

void ADrone::BeginPlay()
{
	Super::BeginPlay();

	PawnMovement->Acceleration = Acceleration;
	PawnMovement->Deceleration = Deceleration;
	PawnMovement->MaxSpeed = MaxSpeed;
}

void ADrone::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &ADrone::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ADrone::MoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, &ADrone::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Turn", this, &ADrone::AddControllerYawInput);
	PlayerInputComponent->BindAxis("MoveUp", this, &ADrone::MoveUp);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ADrone::StartShooting);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &ADrone::StopShooting);

}

#pragma region Movement

void ADrone::MoveForward(float AxisValue)
{
	AddMovementInput(GetActorForwardVector() * AxisValue);
}

void ADrone::MoveRight(float AxisValue)
{
	AddMovementInput(GetActorRightVector() * AxisValue);
}

void ADrone::MoveUp(float AxisValue)
{
	AddMovementInput(GetActorUpVector() * AxisValue);
}

#pragma endregion

void ADrone::StartShooting(){
	Fire();
	GetWorldTimerManager().SetTimer(FireRateTimer, this, &ADrone::Fire, FireRate, true);
}

void ADrone::StopShooting(){
	GetWorldTimerManager().ClearTimer(FireRateTimer);
}

void ADrone::Fire(){
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, FString::Printf(TEXT("Fire")));
}
