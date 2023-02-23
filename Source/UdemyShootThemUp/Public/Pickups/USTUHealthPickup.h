// Udemy Shoot Them Up

#pragma once

#include "CoreMinimal.h"
#include "Pickups/USTUBasePickup.h"
#include "USTUHealthPickup.generated.h"

UCLASS()
class UDEMYSHOOTTHEMUP_API AUSTUHealthPickup : public AUSTUBasePickup
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup", meta = (ClampMin = "1.0", ClampMax = "100.0"))
		float HealthAmount = 50.0f;

private:
	virtual bool GivePickupTo(APawn* PlayerPawn) override;
};
