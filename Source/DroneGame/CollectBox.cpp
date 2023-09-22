#include "CollectBox.h"

ACollectBox::ACollectBox() : Super()
{
	PrimaryActorTick.bCanEverTick = true;

	Collider = CreateDefaultSubobject<USphereComponent>("Collider");
	Collider->SetCollisionProfileName("OverlapOnlyPawn");
	Collider->SetSphereRadius(65.0f);
	RootComponent = Collider;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetCollisionProfileName("NoCollision");
	Mesh->AttachToComponent(Collider, FAttachmentTransformRules::KeepRelativeTransform);
}

void ACollectBox::BeginPlay()
{
	Super::BeginPlay();
	Collider->OnComponentBeginOverlap.AddDynamic(this, &ACollectBox::OverlapBegin);
}

void ACollectBox::OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}
