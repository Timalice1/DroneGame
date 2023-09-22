#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Turret.generated.h"

UCLASS()
class DRONEGAME_API ATurret : public APawn
{
	GENERATED_BODY()

public:
	ATurret();

protected:
	virtual void BeginPlay() override;

};
