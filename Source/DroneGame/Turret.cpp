#include "Turret.h"
#include "DroneGameGameModeBase.h"
#include "Medkit.h"
#include "AmmoBox.h"

ATurret::ATurret() : Super()
{
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>("DefaultRoot");
	RootComponent = Root;

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>("Mesh");
	Mesh->SetupAttachment(Root);
	Mesh->SetCollisionProfileName("BlockAll");

	PawnSense = CreateDefaultSubobject<UPawnSensingComponent>("PawnSense");
	PawnSense->SightRadius = FireRange;
}

void ATurret::BeginPlay()
{
	Super::BeginPlay();
	CurrerntHealth = MaxHealth;

	PawnSense->OnSeePawn.AddDynamic(this, &ATurret::OnEnemySeen);
	TargetRotation = FRotator::ZeroRotator;

	FTimerHandle _delay;
	GetWorldTimerManager().SetTimer(_delay, this, &ATurret::StopAttack, FireRate + .1f, true);
}

void ATurret::GetActorEyesViewPoint(FVector& OutLocation, FRotator& OutRotation) const
{
	OutLocation = Mesh->GetSocketLocation("Muzzle");
	OutRotation = Mesh->GetSocketRotation("Muzzle");
}

void ATurret::OnEnemySeen(APawn* Pawn)
{
	if (!bIsAlive) return;

	if (Cast<ADrone>(Pawn)) {

		if (!FireTimer.IsValid()) 
			GetWorldTimerManager().SetTimer(FireTimer, this, &ATurret::Attack, FireRate, true);

		//Find aim offset rotation
		FRotator _lookAtRotation = UKismetMathLibrary::FindLookAtRotation(Mesh->GetSocketLocation("Muzzle"), Pawn->GetActorLocation());
		TargetRotation = UKismetMathLibrary::NormalizedDeltaRotator(_lookAtRotation, GetActorRotation());
	}
}

void ATurret::Attack()
{
	FHitResult _hit;
	FVector _start = Mesh->GetSocketLocation("Muzzle");
	FVector _forwardVector = UKismetMathLibrary::GetForwardVector(Mesh->GetSocketRotation("Muzzle"));
	FVector _end = (_forwardVector * FireRange) + _start;

	FCollisionQueryParams _queryParams = FCollisionQueryParams("FireTrace", false, this);

	DrawDebugLine(GetWorld(), _start, _end, FColor::Red, false, .1f, 0U, 5);
	if (GetWorld()->LineTraceSingleByChannel(_hit, _start, _end, ECC_Visibility, _queryParams)) {
		UGameplayStatics::ApplyDamage(_hit.GetActor(), BaseDamage, GetInstigatorController(), this, UDamageType::StaticClass());
	}
}

void ATurret::StopAttack()
{
	GetWorldTimerManager().ClearTimer(FireTimer);
	TargetRotation = FRotator::ZeroRotator;
}

float ATurret::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (!bIsAlive) return 0.0f;

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
	PawnSense->Deactivate();
	Mesh->SetSimulatePhysics(true);

	//Increase player score
	ADroneGameGameModeBase* _gameMode = (ADroneGameGameModeBase*)UGameplayStatics::GetGameMode(GetWorld());
	_gameMode->IncreaseScore();

	//Clear and invalidate all timers 
	GetWorldTimerManager().ClearAllTimersForObject(this);
}