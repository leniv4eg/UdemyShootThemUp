// Udemy Shoot Them Up

#include "Weapon/USTURifleWeapon.h"
#include <Engine/World.h>
#include <DrawDebugHelpers.h>
#include "Weapon/Components/USTUWeaponFXComponent.h"
#include <NiagaraComponent.h>
#include <NiagaraFunctionLibrary.h>

AUSTURifleWeapon::AUSTURifleWeapon()
{
	WeaponFXComponent = CreateDefaultSubobject<UUSTUWeaponFXComponent>("WeaponFXComponent");
}

void AUSTURifleWeapon::BeginPlay()
{
	Super::BeginPlay();
	check(WeaponFXComponent);
}

void AUSTURifleWeapon::StartFire()
{
	InitMuzzleFX();
	GetWorldTimerManager().SetTimer(ShotTimerHandle, this, &AUSTURifleWeapon::MakeShot, TimeBetweenShots, true);
	MakeShot();
}

void AUSTURifleWeapon::StopFire()
{
	GetWorldTimerManager().ClearTimer(ShotTimerHandle);
	SetMuzzleFxVisibility(false);
}

void AUSTURifleWeapon::MakeShot()
{
	UE_LOG(LogTemp, Display, TEXT("Make shot"));
	if (!GetWorld() || IsAmmoEmpty())
	{
		StopFire();
		return;
	}

	FVector TraceStart, TraceEnd;
	if (!GetTraceData(TraceStart, TraceEnd)) 
	{
		StopFire();
		return;
	}

	FHitResult HitResult;
	MakeHit(HitResult, TraceStart, TraceEnd);

	FVector TraceFXEnd = TraceEnd;
	if (HitResult.bBlockingHit)
	{
		TraceFXEnd = HitResult.ImpactPoint;
		MakeDamage(HitResult);
		WeaponFXComponent->PlayImpactFX(HitResult);
	}
	//else
	//{
	//	DrawDebugLine(GetWorld(), GetMuzzleWorldLocation(), TraceEnd, FColor::Red, false, 3.0f, 0, 3.0f);
	//}
	SpawnTraceFX(GetMuzzleWorldLocation(), TraceFXEnd);
	DecreaseAmmo();
}

bool AUSTURifleWeapon::GetTraceData(FVector& TraceStart, FVector& TraceEnd) const
{
	FVector ViewLocation;
	FRotator ViewRotation;
	if (!GetPlayerViewPoint(ViewLocation, ViewRotation)) return false;

	TraceStart = ViewLocation;
	const auto HalfRad = FMath::DegreesToRadians(BulletSpread);
	const FVector ShootDirection = FMath::VRandCone(ViewRotation.Vector(), HalfRad);
	TraceEnd = TraceStart + ShootDirection * TraceMaxDistance;
	return true;
}

void AUSTURifleWeapon::MakeDamage(FHitResult& HitResult)
{
	const auto DamagedActor = HitResult.GetActor();
	if (!DamagedActor) return;

	DamagedActor->TakeDamage(DamageAmount, FDamageEvent{}, GetPlayerController(), this);
}

void AUSTURifleWeapon::InitMuzzleFX()
{
	if (!MuzzleFXComponent)
	{
		MuzzleFXComponent = SpawnMuzzleFX();
	}
	SetMuzzleFxVisibility(true);
}

void AUSTURifleWeapon::SetMuzzleFxVisibility(bool Visible)
{
	if (MuzzleFXComponent)
	{
		MuzzleFXComponent->SetPaused(!Visible);
		MuzzleFXComponent->SetVisibility(Visible, true);
	}
}

void AUSTURifleWeapon::SpawnTraceFX(const FVector& TraceStart, const FVector& TraceEnd)
{
	const auto TraceFXComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), TraceFX, TraceStart);
	if (TraceFXComponent)
		TraceFXComponent->SetNiagaraVariableVec3(TraceTargetName, TraceEnd);
}