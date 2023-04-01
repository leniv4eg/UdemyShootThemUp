// Udemy Shoot Them Up

#pragma once

#include "CoreMinimal.h"
#include "Weapon/USTUBaseWeapon.h"
#include "USTURifleWeapon.generated.h"

class UUSTUWeaponFXComponent;

UCLASS()
class UDEMYSHOOTTHEMUP_API AUSTURifleWeapon : public AUSTUBaseWeapon
{
	GENERATED_BODY()
	
public:
	AUSTURifleWeapon();
	virtual void StartFire();
	virtual void StopFire();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Damage", meta = (ClampMin = "0.0", ClampMax = "1.0"))
		float TimeBetweenShots = 0.1f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Damage", meta = (ClampMin = "0.0", ClampMax = "30.0"))
		float BulletSpread = 1.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon", meta = (ClampMin = "1.0", ClampMax = "200.0"))
		float DamageAmount = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VFX")
		UUSTUWeaponFXComponent* WeaponFXComponent;

	virtual void BeginPlay() override;
	virtual void MakeShot();
	virtual bool GetTraceData(FVector& TraceStart, FVector& TraceEnd) const override;

private:
	FTimerHandle ShotTimerHandle;

	void MakeDamage(FHitResult& HitResult);
};
