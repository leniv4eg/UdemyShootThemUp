// Udemy Shoot Them Up


#include "USTUGameModeBase.h"
#include "Player/USTUBaseCharacter.h"
#include "Player/USTUPlayerController.h"
#include "UI/USTUGameHUD.h"

AUSTUGameModeBase::AUSTUGameModeBase()
{
	//��������� �������� �� ��������� ��� Pawn'a � Controller'a
	DefaultPawnClass = AUSTUBaseCharacter::StaticClass();
	PlayerControllerClass = AUSTUPlayerController::StaticClass();
	HUDClass = AUSTUGameHUD::StaticClass();
}
