// Udemy Shoot Them Up

#include "Components/USTUHealthComponent.h"
#include "GameFramework/Actor.h"
#include <Engine/World.h>
#include <TimerManager.h>

DEFINE_LOG_CATEGORY_STATIC(LogHealthComponent, All, All)

UUSTUHealthComponent::UUSTUHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UUSTUHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	check(MaxHealth > 0);

	SetHealth(MaxHealth);

	AActor* ComponentOwner = GetOwner();
	if (ComponentOwner)
		ComponentOwner->OnTakeAnyDamage.AddDynamic(this, &UUSTUHealthComponent::OnTakeAneDamage);
}

void UUSTUHealthComponent::OnTakeAneDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (Damage <= 0.0f || IsDead() || !GetWorld()) return;

	SetHealth(Health - Damage);

	if (IsDead())
		OnDeath.Broadcast();	
	else if (AutoHeal)
	{
		GetWorld()->GetTimerManager().SetTimer(HealTimerHandle, this, &UUSTUHealthComponent::HealUpdate, HealUpdateTime, true, HealDelay);
	}
}

void UUSTUHealthComponent::HealUpdate()
{
	SetHealth(Health + HealModifier);

	if (FMath::IsNearlyEqual(Health, MaxHealth) && GetWorld())
		GetWorld()->GetTimerManager().ClearTimer(HealTimerHandle);
}

void UUSTUHealthComponent::SetHealth(float NewHealth)
{
	Health = FMath::Clamp(NewHealth, 0.0f, MaxHealth);
	OnHealtChanged.Broadcast(Health);
}
