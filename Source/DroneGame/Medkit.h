#pragma once

#include "CoreMinimal.h"
#include "CollectBox.h"
#include "Medkit.generated.h"

UCLASS()
class DRONEGAME_API AMedkit : public ACollectBox
{
	GENERATED_BODY()

public:
	AMedkit();

protected:

	virtual void BeginPlay() override;

	virtual void OverlapBegin(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult) override;
	
	UPROPERTY(EditAnywhere, meta = (ClampMin = 0))
	float RestoreValue = 200.0f;
	
	UPROPERTY(EditAnywhere, meta = (ClampMin = 0))
	float RandomDeviation = 50.0f;

};
