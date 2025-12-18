#include "MyFPSPlayerState.h"
#include "FPS.h"

AMyFPSPlayerState::AMyFPSPlayerState()
{
	// 初始化分数 (使用父类的 Score 变量，默认为 0，通常不需要手动重置，但为了演示可以写)
	// Score = 0.0f; 
}

void AMyFPSPlayerState::AddScore(float Amount)
{
	// 直接修改父类 APlayerState 的 Score 变量
	// 注意：在网络同步中，这行代码应该只在服务器运行（我们后续会加 HasAuthority 判断）
	SetScore(GetScore() + Amount);
	
	UE_LOG(LogFPS, Log, TEXT("Player %s Score Updated: New Score = %f (Added %f)"), *GetPlayerName(), GetScore(), Amount);
}

FString AMyFPSPlayerState::GetFPSScoreText() const
{
	// 格式化为 "Score: 123" 的形式
	// 使用 GetScore() 获取父类分数
	return FString::Printf(TEXT("Score: %.0f"), GetScore());
}
