#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShooterNPCSpawner.generated.h"

class UCapsuleComponent;
class UArrowComponent;
class AShooterNPC;

/**
 *  负责生成射击游戏 NPC 并监控其死亡的基础 Actor。
 *  NPC 将被逐个生成，生成器将等待当前 NPC 死亡后再生成新的一名 NPC。
 */
UCLASS()
class FPS_DEMO_API AShooterNPCSpawner : public AActor
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	UCapsuleComponent* SpawnCapsule;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UArrowComponent* SpawnDirection;

protected:

	/** 要生成的 NPC 类型 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="NPC Spawner")
	TSubclassOf<AShooterNPC> NPCClass;

	/** 游戏开始时在生成第一名 NPC 前等待的时间 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="NPC Spawner", meta = (ClampMin = 0, ClampMax = 10))
	float InitialSpawnDelay = 5.0f;

	/** 要生成的 NPC 数量 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="NPC Spawner", meta = (ClampMin = 0, ClampMax = 100))
	int32 SpawnCount = 1;

	/** 当前 NPC 死亡后在生成下一名 NPC 前等待的时间 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="NPC Spawner", meta = (ClampMin = 0, ClampMax = 10))
	float RespawnDelay = 5.0f;

	/** 用于延迟生成 NPC 的计时器 */
	FTimerHandle SpawnTimer;

public:	
	
	/** 构造函数 */
	AShooterNPCSpawner();

public:

	/** 初始化 */
	virtual void BeginPlay() override;

	/** 清理 */
	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;

protected:

	/** 生成一名 NPC 并订阅其死亡事件 */
	void SpawnNPC();

	/** 当生成的 NPC 死亡时调用 */
	UFUNCTION()
	void OnNPCDied();

};
