#include "AmmoBox.h"

AAmmoBox::AAmmoBox() : Super()
{
	PrimaryActorTick.bCanEverTick = true;
	static ConstructorHelpers::FObjectFinder<UStaticMesh>MeshAsset(TEXT("/Script/Engine.StaticMesh'/Game/Assets/Meshes/AmmoBox/AmmoBox.AmmoBox'"));
	Mesh->SetStaticMesh(MeshAsset.Object);
}

void AAmmoBox::BeginPlay()
{
	Super::BeginPlay();
}

void AAmmoBox::OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	ADrone* _drone = (ADrone*) OtherActor;
	if(_drone->RestoreAmmo())
		Destroy();
}
