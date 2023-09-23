#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Turret.h"
#include <Kismet/GameplayStatics.h>
#include "DroneGameGameModeBase.generated.h"

UCLASS()
class DRONEGAME_API ADroneGameGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

	int Score = 0;
	TArray<AActor*> TotalTurrets;

public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = Score)
	int GetScore();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = Score)
	int GetTotalTurrets();

	void IncreaseScore();

protected:

	virtual void BeginPlay() override;
	void FinishGame();

};