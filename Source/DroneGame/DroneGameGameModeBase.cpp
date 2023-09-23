#include "DroneGameGameModeBase.h"

void ADroneGameGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATurret::StaticClass(), TotalTurrets);
}

int ADroneGameGameModeBase::GetScore()
{
	return Score;
}

int ADroneGameGameModeBase::GetTotalTurrets()
{
	return TotalTurrets.Num();
}

void ADroneGameGameModeBase::IncreaseScore()
{
	Score++;

	UE_LOG(LogTemp,Warning, TEXT("%u"), Score);

	if (Score == TotalTurrets.Num()) FinishGame();
}

void ADroneGameGameModeBase::FinishGame()
{
	GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Green, "Clear");
}