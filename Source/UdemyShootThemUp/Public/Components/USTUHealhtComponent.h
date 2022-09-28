// Udemy Shoot Them Up

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "USTUCoreTypes.h"
#include "USTUHealhtComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UDEMYSHOOTTHEMUP_API UUSTUHealhtComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UUSTUHealhtComponent();

	FOnDeathSignature OnDeath;
	FOnHealtChangedSignature OnHealtChanged;

	UFUNCTION(BlueprintCallable)
		bool IsDead() const { return FMath::IsNearlyZero(Health); }

	float GetHealth() const { return Health; }

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category= "Health", meta = (ClampMin = "0.0", ClampMax = "1000.0"))
		float MaxHealth = 100.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal")
		bool AutoHeal = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal", meta = (EditCondition = "AutoHeal"), meta = (ClampMin = "0.0", ClampMax = "1.0"))
		float HealUpdateTime = 0.3f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal", meta = (EditCondition = "AutoHeal"), meta = (ClampMin = "0.0", ClampMax = "5.0"))
		float HealDelay = 3.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal", meta = (EditCondition = "AutoHeal"), meta = (ClampMin = "0.0", ClampMax = "10.0"))
		float HealModifier = 1.0f;

	virtual void BeginPlay() override;

private:
	float Health = 0.0f;
	FTimerHandle HealTimerHandle;

	UFUNCTION()
		void OnTakeAneDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	void HealUpdate();
	void SetHealth(float NewHealth);
};
