// Udemy Shoot Them Up

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "USTUGameHUD.generated.h"

/**
 * 
 */
UCLASS()
class UDEMYSHOOTTHEMUP_API AUSTUGameHUD : public AHUD
{
	GENERATED_BODY()

public:
	virtual void DrawHUD() override;
	
private:
	void DrawCrossHair();
};
