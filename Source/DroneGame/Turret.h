#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Kismet/KismetMathLibrary.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Drone.h"
#include "Turret.generated.h"

UCLASS()
class DRONEGAME_API ATurret : public APawn
{
	GENERATED_BODY()

	UPROPERTY()
	USceneComponent* Root;

	UPROPERTY(EditDefaultsOnly, Category = Mesh)
	USkeletalMeshComponent* Mesh;

	UAIPerceptionComponent* AI_Perceprion;
	
	UAISenseConfig_Sight* Sight_Config;

	FTimerHandle FireTimer;

	ADrone* Enemy;

	float CurrerntHealth;

	bool bIsAlive = true;
	
public:
	ATurret();

protected:
	virtual void BeginPlay() override;

	virtual void GetActorEyesViewPoint(FVector& OutLocation, FRotator& OutRotation) const;

	UPROPERTY(EditAnywhere, meta = (ClampMin = 0), Category = Stats)
	float MaxHealth;

	UPROPERTY(EditAnywhere, meta = (ClampMin = 0), Category = Stats)
	float BaseDamage;

	UPROPERTY(EditAnywhere, meta = (ClampMin = 0), Category = Stats)
	float FireRange;

	UPROPERTY(EditAnywhere, meta = (ClampMin = 0), Category = Stats)
	float FireRate;

	/*Delta rotation for aim offset*/
	UPROPERTY(BlueprintReadOnly, Category = AimOffset)
	FRotator TargetRotation;

	UPROPERTY(EditDefaultsOnly, Category = FX)
	UParticleSystem* MuzzleFlash;

	UPROPERTY(EditDefaultsOnly, Category = FX)
	USoundBase* ShootSound;
	
	UPROPERTY(EditDefaultsOnly, Category = FX)
	UParticleSystem* ExplosionParticles;

	UPROPERTY(EditDefaultsOnly, Category = FX)
	USoundBase* ExplosionSound;
	
	UFUNCTION()
	void OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

	void Attack();

	void StopAttack();
	
	void Death();

	virtual float TakeDamage(float DamageAmount,
		struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator,
		AActor* DamageCauser) override;
};
