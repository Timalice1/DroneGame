#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"
#include "Perception/PawnSensingComponent.h"
#include "Turret.generated.h"

UCLASS()
class DRONEGAME_API ATurret : public APawn
{
	GENERATED_BODY()

	UPROPERTY()
	USceneComponent* Root;

	UPROPERTY(EditDefaultsOnly, Category = Mesh)
	USkeletalMeshComponent* Mesh;

	UPROPERTY()
	UPawnSensingComponent* PawnSense;
	
	FTimerHandle FireTimer;

	float CurrerntHealth;

	bool bIsAlive = true;
	
public:
	ATurret();

protected:
	virtual void BeginPlay() override;

	virtual void GetActorEyesViewPoint(FVector& OutLocation, FRotator& OutRotation) const;

	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = 0), Category = Stats)
	float MaxHealth = 1000.0f;

	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = 0), Category = Stats)
	float BaseDamage = 200.0f;

	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = 0), Category = Stats)
	float FireRange = 5000.0f;

	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = 0), Category = Stats)
	float FireRate = 0.5f;

	/*Delta rotation for aim offset*/
	UPROPERTY(BlueprintReadOnly, Category = AimOffset)
	FRotator TargetRotation;
	
	UPROPERTY(editDefaultsOnly, Category = FX)
	UParticleSystem* ExplosionParticles;

	UPROPERTY(editDefaultsOnly, Category = FX)
	USoundBase* ExplosionSound;

	UFUNCTION()
	void OnEnemySeen(APawn* Pawn);

	void Attack();

	void StopAttack();

	void Death();

	virtual float TakeDamage(float DamageAmount,
		struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator,
		AActor* DamageCauser) override;
};
