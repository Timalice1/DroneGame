#include "Turret.h"
#include "DrawDebugHelpers.h"
#include "AmmoBox.h"
#include "Medkit.h"

ATurret::ATurret() : Super()
{
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>("DefaultRoot");
	RootComponent = Root;

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>("Mesh");
	Mesh->SetupAttachment(Root);
	Mesh->SetCollisionProfileName("Pawn");

	Healthbar = CreateDefaultSubobject<UWidgetComponent>("HealthBarWidget");
	Healthbar->AttachToComponent(Mesh, FAttachmentTransformRules::KeepRelativeTransform);

}

void ATurret::BeginPlay()
{
	Super::BeginPlay();
	Healthbar->SetVisibility(false);

	CurrerntHealth = MaxHealth;

	GetWorldTimerManager().SetTimer(ShootingTimer, this, &ATurret::Attack, 2, true);

}

void ATurret::Attack()
{
	FHitResult _hit;
	FVector _start = Mesh->GetSocketLocation("Muzzle");
	FVector _forwardVector = UKismetMathLibrary::GetForwardVector(Mesh->GetSocketRotation("Muzzle"));
	FVector _end = (_forwardVector * FireRange) + _start;

	FCollisionQueryParams _queryParams = FCollisionQueryParams("FireTrace", false, this);

	DrawDebugLine(GetWorld(), _start, _end, FColor::Red, false, 1, 0U, 5);
	if (GetWorld()->LineTraceSingleByChannel(_hit, _start, _end, ECC_Visibility, _queryParams)) {

	}
}

float ATurret::GetCurrentHealth()
{
	return CurrerntHealth;
}

float ATurret::GetMaxHealth()
{
	return MaxHealth;
}

float ATurret::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (!bIsAlive) return 0.0f;

	GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, "Damaged");
	Healthbar->SetVisibility(true);

	CurrerntHealth -= DamageAmount;

	if (CurrerntHealth <= 0) Death();

	return DamageAmount;
}

void ATurret::Death()
{
	//Play FX
	FTransform _explosionTransform = FTransform(FRotator::ZeroRotator, GetActorLocation(), FVector(10, 10, 10));
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionParticles, _explosionTransform);
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), ExplosionSound, GetActorLocation());

	//Drop loot
	FVector _spawnLocation = GetActorLocation() + GetActorForwardVector() * 500;
	if (FMath::RandBool()) {
		if (FMath::RandBool())
			GetWorld()->SpawnActor<AAmmoBox>(_spawnLocation, FRotator::ZeroRotator);
		else 
			GetWorld()->SpawnActor<AMedkit>(_spawnLocation, FRotator::ZeroRotator);
	}

	bIsAlive = false;
	Mesh->SetSimulatePhysics(true);
	GetWorldTimerManager().ClearTimer(ShootingTimer);

}


