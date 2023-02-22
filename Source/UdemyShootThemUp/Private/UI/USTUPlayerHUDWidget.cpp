// Udemy Shoot Them Up


#include "UI/USTUPlayerHUDWidget.h"
#include "Components/USTUHealthComponent.h"
#include "Components/USTUWeaponComponent.h"
#include "USTUUtils.h"

float UUSTUPlayerHUDWidget::GetHealthPercent() const
{
	const auto HealthComponent = USTUUtils::GetPlayerComponent<UUSTUHealthComponent>(GetOwningPlayerPawn());
	if (!HealthComponent) return 0.0f;

	return HealthComponent->GetHealthPercent();
}

bool UUSTUPlayerHUDWidget::GetCurrentWeaponUIData(FWeaponUIData& UIData) const
{
	const auto WeaponComponent = USTUUtils::GetPlayerComponent<UUSTUWeaponComponent>(GetOwningPlayerPawn());
	if (!WeaponComponent) return false;

	return WeaponComponent->GetCurrentWeaponUIData(UIData);
}

bool UUSTUPlayerHUDWidget::GetCurrentWeaponAmmo(FAmmoData& AmmoData) const
{
	const auto WeaponComponent = USTUUtils::GetPlayerComponent<UUSTUWeaponComponent>(GetOwningPlayerPawn());
	if (!WeaponComponent) return false;

	return WeaponComponent->GetCurrentWeaponAmmoData(AmmoData);
}

bool UUSTUPlayerHUDWidget::IsPlayerAlive() const
{
	const auto HealthComponent = USTUUtils::GetPlayerComponent<UUSTUHealthComponent>(GetOwningPlayerPawn());
	return HealthComponent && !HealthComponent->IsDead();
}

bool UUSTUPlayerHUDWidget::IsPlayerSpectating() const
{
	const auto Controller = GetOwningPlayer();
	return Controller && Controller->GetStateName() == NAME_Spectating;
}