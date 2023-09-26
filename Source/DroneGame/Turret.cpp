#include "Turret.h"
#include "DroneGameGameModeBase.h"
#include "Medkit.h"
#include "AmmoBox.h"

ATurret::ATurret() : Super()
{
	PrimaryActorTick.bCanEverTick = true;

	MaxHealth = 2000.f;
	BaseDamage = 100.f;
	FireRange = 5000.f;
	FireRate = .15f;

	Root = CreateDefaultSubobject<USceneComponent>("DefaultRoot");
	RootComponent = Root;

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>("Mesh");
	Mesh->SetupAttachment(Root);
	Mesh->SetCollisionProfileName("BlockAll");


	//Perceprion config
	AI_Perceprion = CreateDefaultSubobject<UAIPerceptionComponent>("AI_Perception");
	
	Sight_Config = CreateDefaultSubobject<UAISenseConfig_Sight>("Sight sense");
	Sight_Config->SightRadius = FireRange;
	Sight_Config->LoseSightRadius = FireRange + 500;
	Sight_Config->PeripheralVisionAngleDegrees = 70.f;
	Sight_Config->SetMaxAge(.01f);
	Sight_Config->DetectionByAffiliation.bDetectNeutrals = true;
	
	AI_Perceprion->ConfigureSense(*Sight_Config);
	AI_Perceprion->OnTargetPerceptionUpdated.AddDynamic(this, &ATurret::OnPerceptionUpdated);
}

void ATurret::BeginPlay()
{
	Super::BeginPlay();
	CurrerntHealth = MaxHealth;
	TargetRotation = FRotator::ZeroRotator;
}

void ATurret::GetActorEyesViewPoint(FVector& OutLocation, FRotator& OutRotation) const
{
	OutLocation = Mesh->GetSocketLocation("Muzzle");
	OutRotation = Mesh->GetSocketRotation("Muzzle");
}

void ATurret::OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (!bIsAlive) return;

	Enemy = Cast<ADrone>(Actor);
	if (Enemy == nullptr) return;


	if (Stimulus.WasSuccessfullySensed()) {
		//Find aim offset rotation
		FRotator _lookAtRotation = UKismetMathLibrary::FindLookAtRotation(Mesh->GetSocketLocation("Muzzle"), Enemy->GetActorLocation());
		TargetRotation = UKismetMathLibrary::NormalizedDeltaRotator(_lookAtRotation, GetActorRotation());
		Mesh->SetRelativeRotation(FRotator(0.f, TargetRotation.Yaw, 0.f));

		if(!FireTimer.IsValid())
			GetWorldTimerManager().SetTimer(FireTimer, this, &ATurret::Attack, FireRate, true);
	}
	else 
		StopAttack();

}

void ATurret::Attack()
{
	FHitResult _hit;
	FVector _start = Mesh->GetSocketLocation("Muzzle");
	FVector _forwardVector = UKismetMathLibrary::GetForwardVector(Mesh->GetSocketRotation("Muzzle"));
	FVector _end = (_forwardVector * FireRange) + _start;

	FCollisionQueryParams _queryParams = FCollisionQueryParams("FireTrace", false, this);

	if (GetWorld()->LineTraceSingleByChannel(_hit, _start, _end, ECC_Visibility, _queryParams)) 
		UGameplayStatics::ApplyDamage(_hit.GetActor(), BaseDamage, GetInstigatorController(), this, UDamageType::StaticClass());

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), ShootSound, Mesh->GetSocketLocation("Muzzle"));
	UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, Mesh, "Muzzle");
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
	Mesh->SetSimulatePhysics(true);

	//Increase player score
	ADroneGameGameModeBase* _gameMode = (ADroneGameGameModeBase*)UGameplayStatics::GetGameMode(GetWorld());
	_gameMode->IncreaseScore();

	//Clear and invalidate all timers 
	GetWorldTimerManager().ClearAllTimersForObject(this);
}