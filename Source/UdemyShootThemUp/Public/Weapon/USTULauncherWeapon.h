// Udemy Shoot Them Up

#pragma once

#include "CoreMinimal.h"
#include "Weapon/USTUBaseWeapon.h"
#include "USTULauncherWeapon.generated.h"

class AUSTUProjectile;

UCLASS()
class UDEMYSHOOTTHEMUP_API AUSTULauncherWeapon : public AUSTUBaseWeapon
{
	GENERATED_BODY()
		
public:
	virtual void StartFire() override;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
		TSubclassOf<AUSTUProjectile> ProjectileClass;

	virtual void MakeShot();
};
