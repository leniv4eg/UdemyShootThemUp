// Udemy Shoot Them Up


#include "USTUGameModeBase.h"
#include "Player/USTUBaseCharacter.h"
#include "Player/USTUPlayerController.h"
#include "UI/USTUGameHUD.h"

AUSTUGameModeBase::AUSTUGameModeBase()
{
	//Установка значений по умолчанию для Pawn'a и Controller'a
	DefaultPawnClass = AUSTUBaseCharacter::StaticClass();
	PlayerControllerClass = AUSTUPlayerController::StaticClass();
	HUDClass = AUSTUGameHUD::StaticClass();
}
