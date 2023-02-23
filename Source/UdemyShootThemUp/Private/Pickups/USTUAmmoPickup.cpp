// Udemy Shoot Them Up

#include "Pickups/USTUAmmoPickup.h"
#include "Components/USTUHealthComponent.h"
#include "Components/USTUWeaponComponent.h"
#include "USTUUtils.h"

DEFINE_LOG_CATEGORY_STATIC(LogAmmoPickup, All, All);

bool AUSTUAmmoPickup::GivePickupTo(APawn* PlayerPawn)
{
	const auto HealthComponent = USTUUtils::GetPlayerComponent<UUSTUHealthComponent>(PlayerPawn);
	if (!HealthComponent || HealthComponent->IsDead()) return false;

	const auto WeaponComponent = USTUUtils::GetPlayerComponent<UUSTUWeaponComponent>(PlayerPawn);
	if (!WeaponComponent) return false;

	return WeaponComponent->TryToAddAmmo(WeaponType, ClipsAmount);
}
