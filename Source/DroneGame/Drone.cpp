#include "Drone.h"
#include "DroneGameGameModeBase.h"


ADrone::ADrone()
{
	PrimaryActorTick.bCanEverTick = true;
	bUseControllerRotationYaw = true;

	MaxSpeed = 1500.f;
	Acceleration = 1000.f;
	Deceleration = 1000.f;
	
	FireRate = .15f;
	FireRange = 5000.f;
	Damage = 100.f;
	RecoilValue = .4f;

	MagazineSize = 50;
	MaxHealth = 2000.f;

	Collider = CreateDefaultSubobject<USphereComponent>("Collider");
	Collider->SetCollisionProfileName("Pawn");
	RootComponent = Collider;

	PawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>("PawnMovement");

	Camera = CreateDefaultSubobject<UCameraComponent>("FPV_Camera");
	Camera->AttachToComponent(Collider, FAttachmentTransformRules::KeepRelativeTransform);
	Camera->bUsePawnControlRotation = true;

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>("DroneMesh");
	Mesh->AttachToComponent(Camera, FAttachmentTransformRules::KeepRelativeTransform);
}

void ADrone::BeginPlay()
{
	Super::BeginPlay();

	PawnMovement->Acceleration = Acceleration;
	PawnMovement->Deceleration = Deceleration;
	PawnMovement->MaxSpeed = MaxSpeed;

	AmmoLeft = MagazineSize;
	CurrentHealth = MaxHealth;
}

//Player input
void ADrone::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &ADrone::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ADrone::MoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, &ADrone::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Turn", this, &ADrone::AddControllerYawInput);
	PlayerInputComponent->BindAxis("MoveUp", this, &ADrone::MoveUp);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ADrone::StartShooting);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &ADrone::StopShooting);
}


bool ADrone::Heal(float HealValue)
{
	if (CurrentHealth < MaxHealth) {
		CurrentHealth += HealValue;
		return true;
	}
	return false;
}

bool ADrone::RestoreAmmo()
{
	if (AmmoLeft < MagazineSize) {
		AmmoLeft = MagazineSize;
		return true;
	}

	return false;
}

//Movements
void ADrone::MoveForward(float AxisValue)
{
	AddMovementInput(GetActorForwardVector() * AxisValue);
}

void ADrone::MoveRight(float AxisValue)
{
	AddMovementInput(GetActorRightVector() * AxisValue);
}

void ADrone::MoveUp(float AxisValue)
{
	AddMovementInput(GetActorUpVector() * AxisValue);
}

//Shooting
void ADrone::StartShooting(){

	if (AmmoLeft == 0) {
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), emptySound, GetActorLocation());
		StopShooting();
		return;
	}

	Fire();
	GetWorldTimerManager().SetTimer(FireTimer, this, &ADrone::Fire, FireRate, true);
}

void ADrone::StopShooting(){
	GetWorldTimerManager().ClearTimer(FireTimer);
}

void ADrone::Fire(){

	if (AmmoLeft == 0) {
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), emptySound, GetActorLocation());
		StopShooting();
		return;
	}

	FHitResult _hit;

	FVector _start = Camera->GetComponentLocation();
	FVector _end = _start + (Camera->GetForwardVector() * FireRange);

	FCollisionQueryParams _queryParams = FCollisionQueryParams("FireTrace", false, this);

	if (GetWorld()->LineTraceSingleByChannel(_hit, _start, _end, ECC_Visibility, _queryParams)) 
		UGameplayStatics::ApplyDamage(_hit.GetActor(), Damage, GetInstigatorController(), this, UDamageType::StaticClass());

	AmmoLeft--;
	//FX
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), fireSound, GetActorLocation());
	UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, Mesh, "Muzzle");

	//Recoil
	AddControllerPitchInput(-RecoilValue);
	AddControllerYawInput(FMath::RandRange(- RecoilValue, RecoilValue));
}

int ADrone::GetAmmoLeft()
{
	return AmmoLeft;
}

float ADrone::GetCurrentHealth()
{
	return CurrentHealth;
}

//Damage
float ADrone::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	CurrentHealth -= DamageAmount;
	if (CurrentHealth <= 0) Death();

	return DamageAmount;
}

void ADrone::Death()
{
	ADroneGameGameModeBase* _gameMode = (ADroneGameGameModeBase*)UGameplayStatics::GetGameMode(GetWorld());
	_gameMode->FinishGame();
}