#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "FPSCharacter.h"  // 模板角色
#include "FPSPlayerController.h"  // 模板控制器
#include "MyFPSGameMode.generated.h"

UCLASS()
class FPS_API AMyFPSGameMode : public AGameMode
{
	GENERATED_BODY()
public:
	AMyFPSGameMode();  // 构造器

protected:
	virtual void BeginPlay() override;  // ← 关键！override
};
