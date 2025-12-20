#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ShooterGameMode.generated.h"

class UShooterUI;

/**
 *  第一人称射击游戏的基础 GameMode（游戏模式）
 *  管理游戏 UI
 *  跟踪阵营得分
 */
UCLASS(abstract)
class FPS_DEMO_API AShooterGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
protected:

	/** 要生成的 UI 微件（Widget）类型 */
	UPROPERTY(EditAnywhere, Category="Shooter")
	TSubclassOf<UShooterUI> ShooterUIClass;

	/** 指向 UI 微件的指针 */
	TObjectPtr<UShooterUI> ShooterUI;

	/** 按阵营 ID 映射的分数表 */
	TMap<uint8, int32> TeamScores;

protected:

	/** 游戏开始初始化 */
	virtual void BeginPlay() override;

public:

	/** 增加给定阵营的分数 */
	void IncrementTeamScore(uint8 TeamByte);
};
