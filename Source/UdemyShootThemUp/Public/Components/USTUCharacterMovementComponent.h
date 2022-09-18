// Udemy Shoot Them Up

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "USTUCharacterMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class UDEMYSHOOTTHEMUP_API UUSTUCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement", meta = (ClampMin = "1.0", ClampMax = "3.0"))
		float RunModifier = 2.0f;

	virtual float GetMaxSpeed() const override;
};
