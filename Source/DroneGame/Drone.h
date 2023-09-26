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

	FTimerHandle FireTimer;

	int AmmoLeft;

	float CurrentHealth;

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
	float MaxSpeed;

	UPROPERTY(EditAnywhere, meta = (ClampMin = 0), Category = Movement)
	float Acceleration;

	UPROPERTY(EditAnywhere, meta = (ClampMin = 0), Category = Movement)
	float Deceleration;

	UPROPERTY(EditAnywhere, meta = (ClampMin = 0), Category = Weapon)
	float FireRate;

	UPROPERTY(EditAnywhere, meta = (ClampMin = 0), Category = Weapon)
	float Damage;

	UPROPERTY(EditAnywhere, meta = (ClampMin = 0), Category = Weapon)
	float FireRange;

	UPROPERTY(EditDefaultsOnly, Category = Weapon, meta = (ClampMin = 0, ClampMax = 1))
	float RecoilValue;

	UPROPERTY(EditDefaultsOnly, Category = FX)
	USoundBase* fireSound;

	UPROPERTY(EditDefaultsOnly, Category = FX)
	USoundBase* emptySound;
	
	UPROPERTY(EditDefaultsOnly, Category = FX)
	UParticleSystem* MuzzleFlash;
	

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = 0), Category = Weapon)
	int MagazineSize = 50;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = 0), Category = Stats)
	float MaxHealth = 1000.0f;

	UFUNCTION(BlueprintPure, Category = Stats)
	int GetAmmoLeft();
	
	UFUNCTION(BlueprintPure, Category = Stats)
	float GetCurrentHealth();

	bool Heal(float HealValue);

	bool RestoreAmmo();

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
};
