// Udemy Shoot Them Up


#include "UI/USTUGameHUD.h"
#include <Engine/Canvas.h>
#include <Blueprint/UserWidget.h>

void AUSTUGameHUD::DrawHUD()
{
	Super::DrawHUD();
	//DrawCrossHair();
}

void AUSTUGameHUD::BeginPlay()
{
	Super::BeginPlay();
	auto PlayerHUDWidget = CreateWidget<UUserWidget>(GetWorld(), PlayerHUDWidgetClass);
	if (PlayerHUDWidget)
	{
		PlayerHUDWidget->AddToViewport();
	}
}

void AUSTUGameHUD::DrawCrossHair()
{
	const TInterval<float> Center(Canvas->SizeX / 2, Canvas->SizeY / 2);

	const float HalfLineSize = 10.0f;
	const float LineThickness = 2.0f;
	const FLinearColor LineColor = FLinearColor::Green;

	DrawLine(Center.Min - HalfLineSize, Center.Max, Center.Min + HalfLineSize, Center.Max, LineColor, LineThickness);
	DrawLine(Center.Min, Center.Max - HalfLineSize, Center.Min, Center.Max + HalfLineSize, LineColor, LineThickness);
}
