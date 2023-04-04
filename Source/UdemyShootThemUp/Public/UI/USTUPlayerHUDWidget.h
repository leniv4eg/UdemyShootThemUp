// Udemy Shoot Them Up

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "USTUCoreTypes.h"
#include "USTUPlayerHUDWidget.generated.h"

UCLASS()
class UDEMYSHOOTTHEMUP_API UUSTUPlayerHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "UI")
		float GetHealthPercent() const;

	UFUNCTION(BlueprintCallable, Category = "UI")
		bool GetCurrentWeaponUIData(FWeaponUIData& UIData) const;
	
	UFUNCTION(BlueprintCallable, Category = "UI")
		bool GetCurrentWeaponAmmo(FAmmoData& AmmoData) const;
	
	UFUNCTION(BlueprintCallable, Category = "UI")
		bool IsPlayerAlive() const;

	UFUNCTION(BlueprintCallable, Category = "UI")
		bool IsPlayerSpectating() const;

	UFUNCTION(BlueprintImplementableEvent, Category = "UI")
		void OnTakeDamage_BP();

	virtual bool Initialize() override;

private:
	void OnHealthChanged(float Health, float HealthDelta);
};
