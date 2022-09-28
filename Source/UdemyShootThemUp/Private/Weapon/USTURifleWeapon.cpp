// Udemy Shoot Them Up

#include "Weapon/USTURifleWeapon.h"
#include <Engine/World.h>
#include <DrawDebugHelpers.h>

void AUSTURifleWeapon::StartFire()
{
	GetWorldTimerManager().SetTimer(ShotTimerHandle, this, &AUSTURifleWeapon::MakeShot, TimeBetweenShots, true);
	MakeShot();
}

void AUSTURifleWeapon::StopFire()
{
	GetWorldTimerManager().ClearTimer(ShotTimerHandle);
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

	if (HitResult.bBlockingHit)
	{
		/*//const FVector MuzzleLocation = SocketTransform.GetLocation();
		//const FVector VectorCameraToMuzzle = MuzzleLocation - ViewLocation;
		//const FVector ProjectedVector = VectorCameraToMuzzle.ProjectOnToNormal(ViewRotation.Vector());
		//const FVector Out = ViewLocation - ProjectedVector;

		//DrawDebugLine(GetWorld(), MuzzleLocation, Out, FColor::Red, false, 3.0f, 0, 3.0f);*/
		MakeDamage(HitResult);
		DrawDebugLine(GetWorld(), GetMuzzleWorldLocation(), HitResult.ImpactPoint, FColor::Red, false, 3.0f, 0, 3.0f);
		DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10.0f, 24, FColor::Red, false, 5.0f);
	}
	else
	{
		DrawDebugLine(GetWorld(), GetMuzzleWorldLocation(), TraceEnd, FColor::Red, false, 3.0f, 0, 3.0f);
	}
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