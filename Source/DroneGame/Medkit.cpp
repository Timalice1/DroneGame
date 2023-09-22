#include "Medkit.h"

AMedkit::AMedkit() : Super()
{
	PrimaryActorTick.bCanEverTick = true;
	static ConstructorHelpers::FObjectFinder<UStaticMesh>MeshAsset(TEXT("/Script/Engine.StaticMesh'/Game/Assets/Meshes/Medkit/first_aid_box.first_aid_box'"));
	Mesh->SetStaticMesh(MeshAsset.Object);
	this->RestoreValue = RestoreValue;
}

void AMedkit::BeginPlay()
{
	Super::BeginPlay();
}

void AMedkit::OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	ADrone* drone = (ADrone*)OtherActor;
	if (!(drone->CurrentHealth < drone->MaxHealth)) return;

	drone->CurrentHealth += FMath::RandRange((RestoreValue - RandomDeviation), (RestoreValue + RandomDeviation));
	Destroy();
}
