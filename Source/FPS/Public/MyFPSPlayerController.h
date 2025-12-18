#pragma once

#include "CoreMinimal.h"
#include "FPSPlayerController.h"
#include "MyFPSPlayerController.generated.h"

/**
 * AMyFPSPlayerController
 * 自定义玩家控制器，继承自模版 AFPSPlayerController。
 */
UCLASS()
class FPS_API AMyFPSPlayerController : public AFPSPlayerController
{
	GENERATED_BODY()
	
public:
	AMyFPSPlayerController();

protected:
	virtual void BeginPlay() override;
};
