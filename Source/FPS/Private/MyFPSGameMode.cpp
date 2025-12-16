#include "MyFPSGameMode.h"
#include "MyFPSCharacter.h"
#include "MyFPSPlayerController.h"
#include "MyFPSGameState.h"
#include "MyFPSPlayerState.h"
#include "MyFPSHUD.h"
#include "Kismet/GameplayStatics.h"
#include "FPS.h"

AMyFPSGameMode::AMyFPSGameMode()
{
	// 设置默认Pawn类（玩家角色）
	DefaultPawnClass = AMyFPSCharacter::StaticClass();

	// 设置默认PlayerController类
	PlayerControllerClass = AMyFPSPlayerController::StaticClass();

	// 设置默认HUD CLass（后添加UI用）
	HUDClass = AMyFPSHUD::StaticClass();

	// 使用自定义的 GameState 和 PlayerState
	GameStateClass = AMyFPSGameState::StaticClass();
	PlayerStateClass = AMyFPSPlayerState::StaticClass();
}

void AMyFPSGameMode::BeginPlay()
{
	Super::BeginPlay();

	// Log测试
	UE_LOG(LogFPS, Log, TEXT("MyFPSGameMode BeginPlay!"));
}