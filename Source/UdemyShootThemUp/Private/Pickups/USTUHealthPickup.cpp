// Udemy Shoot Them Up

#include "Pickups/USTUHealthPickup.h"
#include "Components/USTUHealthComponent.h"
#include "USTUUtils.h"

DEFINE_LOG_CATEGORY_STATIC(LogHealthPickup, All, All);

bool AUSTUHealthPickup::GivePickupTo(APawn* PlayerPawn)
{
	const auto HealthComponent = USTUUtils::GetPlayerComponent<UUSTUHealthComponent>(PlayerPawn);
	if (!HealthComponent) return false;

	//UE_LOG(LogHealthPickup, Display, TEXT("Health was taken!"));
	return HealthComponent->TryToAddHealth(HealthAmount);
}
