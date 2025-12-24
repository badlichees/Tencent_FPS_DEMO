#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "FPS_DEMOCameraManager.generated.h"

/**
 *  基础第一人称相机管理器。
 *  限制了仰视/俯视（Pitch）的最小和最大值。
 */
UCLASS()
class AFPS_DEMOCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()
	
public:

	/** 构造函数 */
	AFPS_DEMOCameraManager();
};
