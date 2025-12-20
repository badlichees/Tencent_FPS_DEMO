#include "Variant_Shooter/AI/ShooterAIController.h"
#include "ShooterNPC.h"
#include "Components/StateTreeAIComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "AI/Navigation/PathFollowingAgentInterface.h"

AShooterAIController::AShooterAIController()
{
	// 创建 StateTree 组件
	StateTreeAI = CreateDefaultSubobject<UStateTreeAIComponent>(TEXT("StateTreeAI"));
	StateTreeAI->SetStartLogicAutomatically(false);

	// 创建 AI 感知组件。它将在蓝图中配置
	AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));

	// 订阅 AI 感知委托
	AIPerception->OnTargetPerceptionUpdated.AddDynamic(this, &AShooterAIController::OnPerceptionUpdated);
	AIPerception->OnTargetPerceptionForgotten.AddDynamic(this, &AShooterAIController::OnPerceptionForgotten);
}

void AShooterAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	// 确保我们拥有的是一个 NPC
	if (AShooterNPC* NPC = Cast<AShooterNPC>(InPawn))
	{
		// 为 Pawn 添加阵营标签
		NPC->Tags.Add(TeamTag);

		// 订阅 Pawn 的死亡委托
		NPC->OnPawnDeath.AddDynamic(this, &AShooterAIController::OnPawnDeath);

		// 开始 AI 逻辑
		StateTreeAI->StartLogic();
	}
}

void AShooterAIController::OnPawnDeath()
{
	// 停止移动
	GetPathFollowingComponent()->AbortMove(*this, FPathFollowingResultFlags::UserAbort);

	// 停止 StateTree 逻辑
	StateTreeAI->StopLogic(FString(""));

	// 取消拥有 Pawn
	UnPossess();

	// 销毁此控制器
	Destroy();
}

void AShooterAIController::SetCurrentTarget(AActor* Target)
{
	TargetEnemy = Target;
}

void AShooterAIController::ClearCurrentTarget()
{
	TargetEnemy = nullptr;
}

void AShooterAIController::OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	// 将数据传递给 StateTree 委托钩子
	OnShooterPerceptionUpdated.ExecuteIfBound(Actor, Stimulus);
}

void AShooterAIController::OnPerceptionForgotten(AActor* Actor)
{
	// 将数据传递给 StateTree 委托钩子
	OnShooterPerceptionForgotten.ExecuteIfBound(Actor);
}
