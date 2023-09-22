// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CollectBox.h"
#include "AmmoBox.generated.h"

UCLASS()
class DRONEGAME_API AAmmoBox : public ACollectBox
{
	GENERATED_BODY()

public:
	AAmmoBox();

protected:
	virtual void BeginPlay() override;

	virtual void OverlapBegin(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult) override;
	
};
