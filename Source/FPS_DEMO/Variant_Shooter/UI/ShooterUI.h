#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShooterUI.generated.h"

/**
 *  用于第一人称射击游戏的简单计分板 UI
 */
UCLASS(abstract)
class FPS_DEMO_API UShooterUI : public UUserWidget
{
	GENERATED_BODY()
	
public:

	/** 允许蓝图更新计分子微件 */
	UFUNCTION(BlueprintImplementableEvent, Category="Shooter", meta = (DisplayName = "Update Score"))
	void BP_UpdateScore(uint8 TeamByte, int32 Score);
};
