// Udemy Shoot Them Up


#include "Weapon/USTUBaseWeapon.h"
#include <Components/SkeletalMeshComponent.h>
#include <GameFramework/Character.h>
#include <GameFramework/Controller.h>

DEFINE_LOG_CATEGORY_STATIC(LogBaseWeapon, All, All);

AUSTUBaseWeapon::AUSTUBaseWeapon()
{
	PrimaryActorTick.bCanEverTick = false;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
	SetRootComponent(WeaponMesh);
}

void AUSTUBaseWeapon::BeginPlay()
{
	Super::BeginPlay();

	check(WeaponMesh);
	checkf(DefaultAmmo.Bullets > 0, TEXT("Bullets count couldn't be less or equal zero"));
	checkf(DefaultAmmo.Clips > 0, TEXT("Clips count couldn't be less or equal zero"));
	CurrentAmmo = DefaultAmmo;
}

void AUSTUBaseWeapon::Fire()
{
	MakeShot();
}

void AUSTUBaseWeapon::StartFire()
{

}

void AUSTUBaseWeapon::StopFire()
{

}

void AUSTUBaseWeapon::MakeShot()
{
}

APlayerController* AUSTUBaseWeapon::GetPlayerController() const
{
	const auto Player = Cast<ACharacter>(GetOwner());
	if (!Player) return nullptr;

	return Player->GetController<APlayerController>();
}

bool AUSTUBaseWeapon::GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const
{
	const auto Controller = GetPlayerController();
	if (!Controller) return false;

	Controller->GetPlayerViewPoint(ViewLocation, ViewRotation);
	return true;
}

FVector AUSTUBaseWeapon::GetMuzzleWorldLocation() const
{
	return WeaponMesh->GetSocketLocation(MuzzleSocketName);
}

bool AUSTUBaseWeapon::GetTraceData(FVector& TraceStart, FVector& TraceEnd) const
{
	FVector ViewLocation;
	FRotator ViewRotation;
	if (!GetPlayerViewPoint(ViewLocation, ViewRotation)) return false;

	TraceStart = ViewLocation;
	const FVector ShootDirection = ViewRotation.Vector();
	TraceEnd = TraceStart + ShootDirection * TraceMaxDistance;
	return true;
}

void AUSTUBaseWeapon::MakeHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd)
{
	if (!GetWorld()) return;

	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(GetOwner());

	GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, CollisionParams);
}

void AUSTUBaseWeapon::DecreaseAmmo()
{
	if (CurrentAmmo.Bullets == 0)
	{
		UE_LOG(LogBaseWeapon, Warning, TEXT("Clip is empty"));
		return;
	}
	CurrentAmmo.Bullets--;

	if (IsClipEmpty() && !IsAmmoEmpty())
	{
		StopFire();
		OnClipEmpty.Broadcast();
		//ChangeClip();
	}
}

bool AUSTUBaseWeapon::IsAmmoEmpty() const
{
	return !CurrentAmmo.Infinite && CurrentAmmo.Clips == 0 && IsClipEmpty();
}

bool AUSTUBaseWeapon::IsClipEmpty() const
{
	return CurrentAmmo.Bullets == 0;
}

void AUSTUBaseWeapon::ChangeClip()
{
	if (!CurrentAmmo.Infinite)
	{
		if (CurrentAmmo.Clips == 0)
		{
			UE_LOG(LogBaseWeapon, Warning, TEXT("No more clips"));
			return;
		}
		CurrentAmmo.Clips--;
	}
	CurrentAmmo.Bullets = DefaultAmmo.Bullets;
	UE_LOG(LogBaseWeapon, Display, TEXT("----------- Change Clip -----------"));
}

bool AUSTUBaseWeapon::CanReload() const
{
	return CurrentAmmo.Bullets < DefaultAmmo.Bullets && CurrentAmmo.Clips > 0;
}

void AUSTUBaseWeapon::LogAmmo()
{
	FString AmmoInfo = "Ammo: " + FString::FromInt(CurrentAmmo.Bullets) + " / ";
	AmmoInfo += CurrentAmmo.Infinite ? "Infinite" : FString::FromInt(CurrentAmmo.Clips);
	UE_LOG(LogBaseWeapon, Display, TEXT("%s"), *AmmoInfo);
}