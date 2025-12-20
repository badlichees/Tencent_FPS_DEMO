#include "Variant_Shooter/ShooterGameMode.h"
#include "ShooterUI.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

void AShooterGameMode::BeginPlay()
{
	Super::BeginPlay();

	// 创建 UI
	ShooterUI = CreateWidget<UShooterUI>(UGameplayStatics::GetPlayerController(GetWorld(), 0), ShooterUIClass);
	ShooterUI->AddToViewport(0);
}

void AShooterGameMode::IncrementTeamScore(uint8 TeamByte)
{
	// 检索该阵营的得分（如果存在）
	int32 Score = 0;
	if (int32* FoundScore = TeamScores.Find(TeamByte))
	{
		Score = *FoundScore;
	}

	// 增加给定阵营的分数
	++Score;
	TeamScores.Add(TeamByte, Score);

	// 更新 UI
	ShooterUI->BP_UpdateScore(TeamByte, Score);
}
