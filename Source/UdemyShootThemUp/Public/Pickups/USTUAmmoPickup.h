// Udemy Shoot Them Up

#pragma once

#include "CoreMinimal.h"
#include "Pickups/USTUBasePickup.h"
#include "USTUAmmoPickup.generated.h"
class AUSTUBaseWeapon;

UCLASS()
class UDEMYSHOOTTHEMUP_API AUSTUAmmoPickup : public AUSTUBasePickup
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup", meta = (ClampMin = "1.0", ClampMax = "10.0"))
		int32 ClipsAmount = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup")
		TSubclassOf<AUSTUBaseWeapon> WeaponType;

private:
	virtual bool GivePickupTo(APawn* PlayerPawn) override;
};
