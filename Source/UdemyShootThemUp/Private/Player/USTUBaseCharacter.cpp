// Udemy Shoot Them Up


#include "Player/USTUBaseCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/USTUCharacterMovementComponent.h"
#include "Components/USTUHealthComponent.h"
#include "Components/TextRenderComponent.h"
#include "Components/USTUWeaponComponent.h"
#include <Components/CapsuleComponent.h>
#include <GameFramework/Controller.h>

DEFINE_LOG_CATEGORY_STATIC(BaseCharacterLog, All, All);

// Sets default values
AUSTUBaseCharacter::AUSTUBaseCharacter(const FObjectInitializer& ObjInit) :Super(ObjInit.SetDefaultSubobjectClass<UUSTUCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArmComponent->SetupAttachment(GetRootComponent());
	SpringArmComponent->bUsePawnControlRotation = true;
	SpringArmComponent->SocketOffset = FVector(0.0f, 100.0f, 80.0f);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(SpringArmComponent);

	HealthComponent = CreateDefaultSubobject<UUSTUHealthComponent>("HealthComponent");

	HealthTextComponent = CreateDefaultSubobject<UTextRenderComponent>("HealthTextComponent");
	HealthTextComponent->SetupAttachment(GetRootComponent());
	HealthTextComponent->SetOwnerNoSee(true);

	WeaponComponent = CreateDefaultSubobject<UUSTUWeaponComponent>("WeaponComponent");
}

// Called when the game starts or when spawned
void AUSTUBaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	check(HealthComponent);
	check(HealthTextComponent);
	check(GetCharacterMovement());

	OnHealtChanged(HealthComponent->GetHealth());
	HealthComponent->OnDeath.AddUObject(this, &AUSTUBaseCharacter::OnDeath);
	HealthComponent->OnHealtChanged.AddUObject(this, &AUSTUBaseCharacter::OnHealtChanged);
	
	LandedDelegate.AddDynamic(this, &AUSTUBaseCharacter::OnGroundLanded);
}

// Called every frame
void AUSTUBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AUSTUBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);
	check(WeaponComponent);

	PlayerInputComponent->BindAxis("MoveForward",	this, &AUSTUBaseCharacter::MoveForvard);
	PlayerInputComponent->BindAxis("MoveRight",		this, &AUSTUBaseCharacter::MoveRight);
	PlayerInputComponent->BindAxis("LookUp",		this, &AUSTUBaseCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("TurnAround",	this, &AUSTUBaseCharacter::AddControllerYawInput);
	
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AUSTUBaseCharacter::Jump);
	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &AUSTUBaseCharacter::OnStartRuning);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &AUSTUBaseCharacter::OnStopRuning);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, WeaponComponent, &UUSTUWeaponComponent::StartFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, WeaponComponent, &UUSTUWeaponComponent::StopFire);
	PlayerInputComponent->BindAction("NextWeapon", IE_Pressed, WeaponComponent, &UUSTUWeaponComponent::NextWeapon);
	PlayerInputComponent->BindAction("Reload", IE_Pressed, WeaponComponent, &UUSTUWeaponComponent::Reload);
}

void AUSTUBaseCharacter::MoveForvard(float Amount)
{
	//IsMovingForward = Amount > 0.0f;
	if (Amount == 0.0f) return;
	AddMovementInput(GetActorForwardVector(), Amount);
}

void AUSTUBaseCharacter::MoveRight(float Amount)
{
	if (Amount == 0.0f) return;
	AddMovementInput(GetActorRightVector(), Amount);
}

void AUSTUBaseCharacter::OnStartRuning()
{
	WantsToRun = true;
}

void AUSTUBaseCharacter::OnStopRuning()
{
	WantsToRun = false;
}

void AUSTUBaseCharacter::OnDeath()
{
	UE_LOG(BaseCharacterLog, Error, TEXT("Player %s is dead"), *GetName());
	PlayAnimMontage(DeathAnimMontage);
	WeaponComponent->StopFire();

	GetCharacterMovement()->DisableMovement();

	SetLifeSpan(LifeSpanDeath);

	if (Controller)
		Controller->ChangeState(NAME_Spectating);
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
}

void AUSTUBaseCharacter::OnHealtChanged(float Health)
{
	HealthTextComponent->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), Health)));
}

void AUSTUBaseCharacter::OnGroundLanded(const FHitResult& Hit)
{
	const auto FallVelocityZ = -GetVelocity().Z;
	UE_LOG(BaseCharacterLog, Error, TEXT("On landed: %f"), FallVelocityZ);

	if (FallVelocityZ < LandedDamageVelocity.X) return;

	const auto FinalDamage = FMath::GetMappedRangeValueClamped(LandedDamageVelocity, LandedDamage, FallVelocityZ);
	TakeDamage(FinalDamage, FDamageEvent{}, nullptr, nullptr);
	UE_LOG(BaseCharacterLog, Error, TEXT("FinalDamage: %f"), FinalDamage);

}

bool AUSTUBaseCharacter::IsRunning() const
{
	//return WantsToRun && IsMovingForward && !GetVelocity().IsZero();
	return WantsToRun && !GetVelocity().IsZero();
}

float AUSTUBaseCharacter::GetMovementDirection() const
{
	if (GetVelocity().IsZero()) return 0.0f;

	const auto VelocityNormal = GetVelocity().GetSafeNormal();
	const auto AngleBetween = FMath::Acos(FVector::DotProduct(GetActorForwardVector(), VelocityNormal));
	const auto CrossProduct = FVector::CrossProduct(GetActorForwardVector(), VelocityNormal);
	const auto Degrees = FMath::RadiansToDegrees(AngleBetween);

	return CrossProduct.IsZero() ? Degrees : Degrees * FMath::Sign(CrossProduct.Z);
}