// Udemy Shoot Them Up

#include "Components/USTUHealhtComponent.h"
#include "GameFramework/Actor.h"
#include <Engine/World.h>
#include <TimerManager.h>

DEFINE_LOG_CATEGORY_STATIC(LogHealhtComponent, All, All)

UUSTUHealhtComponent::UUSTUHealhtComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UUSTUHealhtComponent::BeginPlay()
{
	Super::BeginPlay();

	SetHealth(MaxHealth);

	AActor* ComponentOwner = GetOwner();
	if (ComponentOwner)
		ComponentOwner->OnTakeAnyDamage.AddDynamic(this, &UUSTUHealhtComponent::OnTakeAneDamage);
}

void UUSTUHealhtComponent::OnTakeAneDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (Damage <= 0 || IsDead() || !GetWorld()) return;

	SetHealth(Health - Damage);

	if (IsDead())
		OnDeath.Broadcast();	
	else if (AutoHeal)
	{
		GetWorld()->GetTimerManager().SetTimer(HealTimerHandle, this, &UUSTUHealhtComponent::HealUpdate, HealUpdateTime, true, HealDelay);
	}
}

void UUSTUHealhtComponent::HealUpdate()
{
	SetHealth(Health + HealModifier);

	if (FMath::IsNearlyEqual(Health, MaxHealth) && GetWorld())
		GetWorld()->GetTimerManager().ClearTimer(HealTimerHandle);
}

void UUSTUHealhtComponent::SetHealth(float NewHealth)
{
	Health = FMath::Clamp(NewHealth, 0.0f, MaxHealth);
	OnHealtChanged.Broadcast(Health);
}
