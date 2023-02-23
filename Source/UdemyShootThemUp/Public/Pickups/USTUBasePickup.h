// Udemy Shoot Them Up

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "USTUBasePickup.generated.h"

class USphereComponent;

UCLASS()
class UDEMYSHOOTTHEMUP_API AUSTUBasePickup : public AActor
{
	GENERATED_BODY()
	
public:	
	AUSTUBasePickup();

protected:
	UPROPERTY(VisibleAnywhere, Category = "Pickup")
		USphereComponent* CollisionComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup")
		float RespawnTime = 5.0f;

	virtual void BeginPlay() override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
public:	
	virtual void Tick(float DeltaTime) override;

private:
	float RotationYaw = 0.0f;
	virtual bool GivePickupTo(APawn* PlayerPawn);

	void PickupWasTaken();
	void Respawn();
	void GenerateRotationYaw();
};
