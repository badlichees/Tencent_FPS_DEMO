#include "MyFPSPlayerState.h"
#include "FPS.h"

AMyFPSPlayerState::AMyFPSPlayerState()
{
	// 初始化分数
	CurrentScore = 0.0f;
}

void AMyFPSPlayerState::AddScore(float Amount)
{
	CurrentScore += Amount;
	
	UE_LOG(LogFPS, Log, TEXT("Player %s Score Updated: New Score = %f (Added %f)"), *GetPlayerName(), CurrentScore, Amount);
}

FString AMyFPSPlayerState::GetFPSScoreText() const
{
	// 格式化为 "Score: 123" 的形式，%.0f 表示不保留小数
	return FString::Printf(TEXT("Score: %.0f"), CurrentScore);
}
