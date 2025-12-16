#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "MyFPSPlayerState.generated.h"

/**
 * AMyFPSPlayerState
 * 玩家状态类：负责保存单个玩家的游戏数据（如分数）。
 * 使用 "My" 前缀以区分自定义逻辑。
 */
UCLASS()
class FPS_API AMyFPSPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	AMyFPSPlayerState();

protected:
	/** 玩家当前分数 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Score")
	float CurrentScore;

public:
	/** 增加分数的函数 */
	UFUNCTION(BlueprintCallable, Category = "Score")
	void AddScore(float Amount);

	/** 获取当前分数 */
	UFUNCTION(BlueprintCallable, Category = "Score")
	float GetFPSScore() const { return CurrentScore; }

	/** 获取格式化后的分数文本 (UI专用) */
	UFUNCTION(BlueprintPure, Category = "Score")
	FString GetFPSScoreText() const;
};
