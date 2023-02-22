// Udemy Shoot Them Up

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "USTUCoreTypes.h"
#include "USTUBaseWeapon.generated.h"


class USkeletalMeshComponent;

UCLASS()
class UDEMYSHOOTTHEMUP_API AUSTUBaseWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	AUSTUBaseWeapon();

	FOnClipEmptySignature OnClipEmpty;

	virtual void Fire();
	virtual void StartFire();
	virtual void StopFire();

	void ChangeClip();
	bool CanReload() const;

	FWeaponUIData GetUIData() const { return UIData; }

	FAmmoData GetAmmoData() const { return CurrentAmmo; }

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
		FName MuzzleSocketName = "MuzzleSocket";
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon", meta = (ClampMin = "1.0", ClampMax = "20000.0"))
		float TraceMaxDistance = 1500.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
		FAmmoData DefaultAmmo {	15, 10, false };
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
		FWeaponUIData UIData;

	virtual void BeginPlay() override;
	virtual bool GetTraceData(FVector& TraceStart, FVector& TraceEnd) const;

	void MakeShot();
	APlayerController* AUSTUBaseWeapon::GetPlayerController() const;
	bool GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const;
	FVector GetMuzzleWorldLocation() const;
	void MakeHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd);

	void DecreaseAmmo();
	bool IsAmmoEmpty() const;
	bool IsClipEmpty() const;
	void LogAmmo();

private:
	FAmmoData CurrentAmmo;
};
