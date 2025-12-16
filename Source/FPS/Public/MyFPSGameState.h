#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "MyFPSGameState.generated.h"

/**
 * AMyFPSGameState
 * 游戏状态类：负责管理整个游戏层面的状态。
 * 使用 "My" 前缀以区分（或扩展）默认的模版逻辑。
 */
UCLASS()
class FPS_API AMyFPSGameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	AMyFPSGameState();
};
