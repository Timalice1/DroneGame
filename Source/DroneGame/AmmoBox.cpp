#include "AmmoBox.h"

AAmmoBox::AAmmoBox() : Super()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AAmmoBox::BeginPlay()
{
	Super::BeginPlay();
}

void AAmmoBox::OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	ADrone* drone = (ADrone*) OtherActor;
	if (!(drone->AmmoLeft < drone->MagazineSize)) return;

	drone->AmmoLeft = drone->MagazineSize;
	
	Destroy();
}
