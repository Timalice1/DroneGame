#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Blueprint/UserWidget.h"
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

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly);
	USphereComponent* Collider;

	UFloatingPawnMovement* PawnMovement;

	UPROPERTY(EditDefaultsOnly)
	USkeletalMeshComponent* Mesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, meta = (ClampMin = 0), Category = Movement)
	float MaxSpeed = 1200.0f;

	UPROPERTY(EditAnywhere, meta = (ClampMin = 0), Category = Movement)
	float Acceleration = 800.0f;

	UPROPERTY(EditAnywhere, meta = (ClampMin = 0), Category = Movement)
	float Deceleration = 1200.0f;

	UPROPERTY(EditAnywhere, meta = (ClampMin = 0), Category = Weapon)
	float FireRate = .1f;

	UPROPERTY(EditAnywhere, meta = (ClampMin = 0), Category = Weapon)
	float Damage = 100.0f;

	UPROPERTY(EditAnywhere, meta = (ClampMin = 0), Category = Weapon)
	float FireRange = 5000.0f;

	UPROPERTY(EditDefaultsOnly, Category = Weapon, meta = (ClampMin = 0, ClampMax = 1))
	float RecoilValue = .2f;

	UPROPERTY(EditDefaultsOnly, Category = FX)
	USoundBase* fireSound;

	UPROPERTY(EditDefaultsOnly, Category = FX)
	USoundBase* emptySound;
	
	UPROPERTY(EditDefaultsOnly, Category = FX)
	UParticleSystem* MuzzleFlash;
	
	UPROPERTY(EditDefaultsOnly, Category = FX)
	UParticleSystem* Impact;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> DroneHUD_Widget;

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = 0), Category = Weapon)
	int MagazineSize = 50;

	UPROPERTY(BlueprintReadOnly, Category = Weapon)
	int AmmoLeft;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = 0), Category = Stats)
	float MaxHealth = 1000.0f;

	UPROPERTY(BlueprintReadOnly, Category = Stats)
	float CurrentHealth;

//Actions
protected:
	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	void MoveUp(float AxisValue);

	void StartShooting();
	void StopShooting();
	void Fire();

	virtual float TakeDamage(float DamageAmount,
		struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator,
		AActor* DamageCauser) override;
	void Death();


private: 
	TSubclassOf<UDamageType> damageTypeClass = UDamageType::StaticClass();
	FTimerHandle FireTimer;
};
