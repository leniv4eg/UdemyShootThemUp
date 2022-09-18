// Udemy Shoot Them Up

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "USTUWeaponComponent.generated.h"

class AUSTUBaseWeapon;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UDEMYSHOOTTHEMUP_API UUSTUWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UUSTUWeaponComponent();

	void Fire();
	void StartFire();
	void StopFire();
	void NextWeapon();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
		TArray<TSubclassOf<AUSTUBaseWeapon>> WeaponClasses;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
		FName WeaponEquipSocketName = "weapon_rSocket";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
		FName WeaponArmorySocketName = "ArmorySocket";

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:	
	UPROPERTY()
		AUSTUBaseWeapon* CurrentWeapon = nullptr;

	UPROPERTY()
		TArray<AUSTUBaseWeapon*> Weapons;

	int32 CurrenWeaponIndex = 0;

	void SpawnWeapons();
	void AttachWeaponToSocket(AUSTUBaseWeapon* Weapon, USceneComponent* SceneComponent, const FName& SocketName);
	void EquipWeapon(int32 WeaponIndex);
};
