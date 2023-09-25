#include "DroneGameGameModeBase.h"

void ADroneGameGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	Score = 0;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATurret::StaticClass(), Turrets);

	if (DroneHUD_Widget_Class) {
		DroneHUD_Widget = CreateWidget<UUserWidget>(GetWorld(), DroneHUD_Widget_Class);
		DroneHUD_Widget->AddToViewport();
	}
}

int ADroneGameGameModeBase::GetScore()
{
	return Score;
}

int ADroneGameGameModeBase::GetTotalTurrets()
{
	return Turrets.Num();
}

void ADroneGameGameModeBase::IncreaseScore()
{
	Score++;
	if (Score == Turrets.Num()) FinishGame();
}

void ADroneGameGameModeBase::FinishGame()
{
	if (DroneHUD_Widget) 
		DroneHUD_Widget->RemoveFromParent();

	if (GameOverWidget_Class) {
		GameOver_Widget = CreateWidget<UUserWidget>(GetWorld(), GameOverWidget_Class);
		GameOver_Widget->AddToViewport();
	}

	//Change input mode
	APlayerController* _controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	_controller->SetShowMouseCursor(true);
	_controller->SetInputMode(FInputModeUIOnly());

	//Clear and invalidate all timers for player and enemies
	for (auto& _turret : Turrets)
		GetWorldTimerManager().ClearAllTimersForObject(_turret);

	GetWorldTimerManager().ClearAllTimersForObject(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
}