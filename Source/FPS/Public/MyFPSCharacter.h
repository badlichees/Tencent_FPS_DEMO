#pragma once

#include "CoreMinimal.h"
#include "FPSCharacter.h"
#include "MyFPSCharacter.generated.h"

/**
 * AMyFPSCharacter
 * 自定义玩家角色类，继承自模版提供的 AFPSCharacter。
 * 这样我们可以保留模版功能（如移动逻辑），同时在这里添加我们自己的功能（如战斗、得分）。
 */
UCLASS()
class FPS_API AMyFPSCharacter : public AFPSCharacter
{
	GENERATED_BODY()

public:
	AMyFPSCharacter();
	
};
