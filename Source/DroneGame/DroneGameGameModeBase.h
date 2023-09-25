#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include <Kismet/GameplayStatics.h>
#include "Blueprint/UserWidget.h"
#include "Turret.h"
#include "Drone.h"
#include "DroneGameGameModeBase.generated.h"

UCLASS()
class DRONEGAME_API ADroneGameGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

	//Current score (destroyed turrets)
	int Score;

	//All turrets in the level
	TArray<AActor*> Turrets;

protected:

	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSubclassOf<class UUserWidget>  GameOverWidget_Class;

	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSubclassOf<class UUserWidget> DroneHUD_Widget_Class;
	
	UPROPERTY()
	class UUserWidget* GameOver_Widget;

	UPROPERTY()
	class UUserWidget* DroneHUD_Widget;

public:
	UFUNCTION(BlueprintPure, Category = Score)
	int GetScore();

	UFUNCTION(BlueprintPure, Category = Score)
	int GetTotalTurrets();

	void IncreaseScore();

	void FinishGame();
};