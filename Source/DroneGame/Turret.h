#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"
#include "AmmoBox.h"
#include "Medkit.h"
#include "Turret.generated.h"

UCLASS()
class DRONEGAME_API ATurret : public APawn
{
	GENERATED_BODY()

	UPROPERTY()
	USceneComponent* Root;

	UPROPERTY(EditDefaultsOnly, Category = Mesh)
	USkeletalMeshComponent* Mesh;

	float CurrerntHealth;

	bool bIsAlive = true;

	TSubclassOf<UDamageType> damageTypeClass = UDamageType::StaticClass();

public:
	ATurret();

protected:
	virtual void BeginPlay() override;

	//virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = 0), Category = Stats)
	float BaseDamage = 200.0f;

	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = 0), Category = Stats)
	float FireRange = 5000.0f;

	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = 0), Category = Stats)
	float MaxHealth = 1000.0f;

	UPROPERTY(editDefaultsOnly, Category = FX)
	UParticleSystem* ExplosionParticles;

	UPROPERTY(editDefaultsOnly, Category = FX)
	USoundBase* ExplosionSound;

	void Attack();
	void Death();
	virtual float TakeDamage(float DamageAmount,
		struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator,
		AActor* DamageCauser) override;
};
