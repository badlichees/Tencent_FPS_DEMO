#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryContext.h"
#include "EnvQueryContext_Target.generated.h"

/**
 *  自定义 EnvQuery 上下文，返回 NPC 当前瞄准的 Actor
 */
UCLASS()
class FPS_DEMO_API UEnvQueryContext_Target : public UEnvQueryContext
{
	GENERATED_BODY()
	
public:

	/** 为此 EnvQuery 提供上下文位置或 Actor */
	virtual void ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const override;

};
