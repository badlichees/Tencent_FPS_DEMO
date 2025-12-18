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

	// 设置默认HUD CLass（添加UI用）
	HUDClass = AMyFPSHUD::StaticClass();

	// 使用自定义的GameState和PlayerState
	GameStateClass = AMyFPSGameState::StaticClass();
	PlayerStateClass = AMyFPSPlayerState::StaticClass();
}

void AMyFPSGameMode::BeginPlay()
{
	Super::BeginPlay();

	// Log测试
	UE_LOG(LogFPS, Log, TEXT("MyFPSGameMode BeginPlay!"));

	// 启动一个循环定时器，每 1.0 秒执行一次 TestAddScore
	GetWorldTimerManager().SetTimer(TimerHandle_TestScore, this, &AMyFPSGameMode::TestAddScore, 1.0f, true);
}

void AMyFPSGameMode::TestAddScore()
{
	// 遍历所有玩家 (现在是单机，所以只有一个)
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		APlayerController* PC = Iterator->Get();
		if (PC && PC->PlayerState)
		{
			AMyFPSPlayerState* PS = Cast<AMyFPSPlayerState>(PC->PlayerState);
			if (PS)
			{
				// 每秒加 10 分
				PS->AddScore(10.0f);
			}
		}
	}
}