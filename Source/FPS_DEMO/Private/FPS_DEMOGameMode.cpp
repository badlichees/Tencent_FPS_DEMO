#include "FPS_DEMOGameMode.h"

AFPS_DEMOGameMode::AFPS_DEMOGameMode()
{
	// 指定默认的Pawn、PlayerController、PlayerState、GameState和HUD类
	DefaultPawnClass = AFPS_DEMOCharacter::StaticClass();
	PlayerControllerClass = AFPS_DEMOPlayerController::StaticClass();
	PlayerStateClass = AFPS_DEMOPlayerState::StaticClass();
	GameStateClass = AFPS_DEMOGameState::StaticClass();
	HUDClass = AFPS_DEMOHUD::StaticClass();
}

void AFPS_DEMOGameMode::BeginPlay()
{
	Super::BeginPlay(); // Super代表当前类的父类/基类，UE里自带了这些基础类的基类
}

