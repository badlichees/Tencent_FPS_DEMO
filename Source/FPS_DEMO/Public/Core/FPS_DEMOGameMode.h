#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FPS_DEMOGameMode.generated.h"

/**
 *  第一人称游戏的基础 GameMode（游戏模式）
 */
UCLASS(abstract)
class AFPS_DEMOGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AFPS_DEMOGameMode();

protected:
	/** 游戏开始时调用 */
	virtual void BeginPlay() override;
};



