// Udemy Shoot Them Up

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "USTUCoreTypes.h"
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
	void Reload();

	bool GetCurrentWeaponUIData(FWeaponUIData& UIData) const;
	bool GetCurrentWeaponAmmoData(FAmmoData& AmmoData) const;

	bool TryToAddAmmo(TSubclassOf<AUSTUBaseWeapon> WeaponType, int32 ClipsAmount);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
		TArray<FWeaponData> WeaponData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
		FName WeaponEquipSocketName = "weapon_rSocket";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
		FName WeaponArmorySocketName = "ArmorySocket";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
		UAnimMontage* EquipAnimMontage;

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:	
	UPROPERTY()
		AUSTUBaseWeapon* CurrentWeapon = nullptr;

	UPROPERTY()
		TArray<AUSTUBaseWeapon*> Weapons;

	UPROPERTY()
		UAnimMontage* CurrentReloadAnimMontage = nullptr;

	int32 CurrenWeaponIndex = 0;

	bool EquipAnimInProgress = false;
	bool ReloadAnimInProgress = false;

	void SpawnWeapons();
	void AttachWeaponToSocket(AUSTUBaseWeapon* Weapon, USceneComponent* SceneComponent, const FName& SocketName);
	void EquipWeapon(int32 WeaponIndex);

	void PlayAnimMontage(UAnimMontage* Animation);

	void InitAnimations();
	void OnEquipFinished(USkeletalMeshComponent* MeshComponent);
	void OnReloadFinished(USkeletalMeshComponent* MeshComponent);

	bool CanFire() const;
	bool CanEquip() const;
	bool CanReload() const;

	void OnEmptyClip(AUSTUBaseWeapon* AmmoEmptyWeapon);
	void ChangeClip();
};
