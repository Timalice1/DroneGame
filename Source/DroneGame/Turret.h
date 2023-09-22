#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Turret.generated.h"

UCLASS()
class DRONEGAME_API ATurret : public APawn
{
	GENERATED_BODY()

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

	void Attack();

	void Death();

	virtual float TakeDamage(float DamageAmount,
		struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator,
		AActor* DamageCauser) override;

public:

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetCurrentHealth();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetMaxHealth();

private:

	UPROPERTY()
	USceneComponent* Root;

	UPROPERTY(EditDefaultsOnly, Category = Mesh)
	USkeletalMeshComponent* Mesh;

	UPROPERTY(EditDefaultsOnly, Category = Widget)
	UWidgetComponent* Healthbar;

	TSubclassOf<UUserWidget> HealtbarWidget;

	float CurrerntHealth;
	bool bIsAlive = true;

	FTimerHandle ShootingTimer;

	TSubclassOf<UDamageType> damageTypeClass = UDamageType::StaticClass();

};
