#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ShooterAIController.generated.h"

class UStateTreeAIComponent;
class UAIPerceptionComponent;
struct FAIStimulus;

DECLARE_DELEGATE_TwoParams(FShooterPerceptionUpdatedDelegate, AActor*, const FAIStimulus&);
DECLARE_DELEGATE_OneParam(FShooterPerceptionForgottenDelegate, AActor*);

/**
 *  用于第一人称射击游戏敌人的简单 AI 控制器
 */
UCLASS(abstract)
class FPS_DEMO_API AShooterAIController : public AAIController
{
	GENERATED_BODY()
	
	/** 为此 NPC 运行行为状态树（StateTree） */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	UStateTreeAIComponent* StateTreeAI;

	/** 通过视觉、听觉和其他感官检测其他 Actor */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	UAIPerceptionComponent* AIPerception;

protected:

	/** 用于识别 Pawn 敌友的阵营标签 */
	UPROPERTY(EditAnywhere, Category="Shooter")
	FName TeamTag = FName("Enemy");

	/** 当前瞄准的敌人 */
	TObjectPtr<AActor> TargetEnemy;

public:

	/** 当 AI 感知更新时调用。状态树任务委托钩子 */
	FShooterPerceptionUpdatedDelegate OnShooterPerceptionUpdated;

	/** 当 AI 感知被遗忘时调用。状态树任务委托钩子 */
	FShooterPerceptionForgottenDelegate OnShooterPerceptionForgotten;

public:

	/** 构造函数 */
	AShooterAIController();

protected:

	/** Pawn 初始化 */
	virtual void OnPossess(APawn* InPawn) override;

protected:

	/** 当拥有的 Pawn 死亡时调用 */
	UFUNCTION()
	void OnPawnDeath();

public:

	/** 设置当前目标 */
	void SetCurrentTarget(AActor* Target);

	/** 清除当前目标 */
	void ClearCurrentTarget();

	/** 返回当前目标 */
	AActor* GetCurrentTarget() const { return TargetEnemy; };

protected:

	/** 当 AI 感知组件更新给定 Actor 的感知时调用 */
	UFUNCTION()
	void OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

	/** 当 AI 感知组件遗忘给定 Actor 时调用 */
	UFUNCTION()
	void OnPerceptionForgotten(AActor* Actor);
};
