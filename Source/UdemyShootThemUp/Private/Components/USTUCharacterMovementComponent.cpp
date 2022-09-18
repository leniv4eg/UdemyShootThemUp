// Udemy Shoot Them Up


#include "Components/USTUCharacterMovementComponent.h"
#include "Player/USTUBaseCharacter.h"

float UUSTUCharacterMovementComponent::GetMaxSpeed() const
{
	const float MaxSpeed = Super::GetMaxSpeed();
	const AUSTUBaseCharacter* Player = Cast<AUSTUBaseCharacter>(GetPawnOwner());
	return Player && Player->IsRunning() ? MaxSpeed * RunModifier : MaxSpeed;
 }