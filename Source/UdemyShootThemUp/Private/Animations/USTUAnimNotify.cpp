// Udemy Shoot Them Up

#include "Animations/USTUAnimNotify.h"

void UUSTUAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	OnNotified.Broadcast(MeshComp);
	Super::Notify(MeshComp, Animation);
}
