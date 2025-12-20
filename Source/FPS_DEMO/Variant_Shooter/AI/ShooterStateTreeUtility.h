#pragma once

#include "CoreMinimal.h"
#include "StateTreeTaskBase.h"
#include "StateTreeConditionBase.h"

#include "ShooterStateTreeUtility.generated.h"

class AShooterNPC;
class AAIController;
class AShooterAIController;

/**
 *  FStateTreeLineOfSightToTargetCondition 条件的实例数据结构
 */
USTRUCT()
struct FStateTreeLineOfSightToTargetConditionInstanceData
{
	GENERATED_BODY()
	
	/** 瞄准中的角色 */
	UPROPERTY(EditAnywhere, Category = "Context")
	AShooterNPC* Character;

	/** 进行视线检查的目标 */
	UPROPERTY(EditAnywhere, Category = "Condition")
	AActor* Target;

	/** 允许的最大视线锥角（以度为单位） */
	UPROPERTY(EditAnywhere, Category = "Condition")
	float LineOfSightConeAngle = 35.0f;

	/** 垂直视线检查的次数，用于尝试绕过低矮障碍物 */
	UPROPERTY(EditAnywhere, Category = "Condition")
	int32 NumberOfVerticalLineOfSightChecks = 5;

	/** 如果为 true，当角色拥有视线时该条件通过 */
	UPROPERTY(EditAnywhere, Category = "Condition")
	bool bMustHaveLineOfSight = true;
};
STATETREE_POD_INSTANCEDATA(FStateTreeLineOfSightToTargetConditionInstanceData);

/**
 *  状态树（StateTree）条件，用于检查角色是否具有目标的视线
 */
USTRUCT(DisplayName = "Has Line of Sight to Target", Category="Shooter")
struct FStateTreeLineOfSightToTargetCondition : public FStateTreeConditionCommonBase
{
	GENERATED_BODY()

	/** 设置实例数据类型 */
	using FInstanceDataType = FStateTreeLineOfSightToTargetConditionInstanceData;
	virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }

	/** 默认构造函数 */
	FStateTreeLineOfSightToTargetCondition() = default;
	
	/** 测试状态树条件 */
	virtual bool TestCondition(FStateTreeExecutionContext& Context) const override;

#if WITH_EDITOR
	/** 提供描述字符串 */
	virtual FText GetDescription(const FGuid& ID, FStateTreeDataView InstanceDataView, const IStateTreeBindingLookup& BindingLookup, EStateTreeNodeFormatting Formatting = EStateTreeNodeFormatting::Text) const override;
#endif

};

////////////////////////////////////////////////////////////////////

/**
 *  面向 Actor 状态树任务的实例数据结构
 */
USTRUCT()
struct FStateTreeFaceActorInstanceData
{
	GENERATED_BODY()

	/** 用于决定焦点 Actor 的 AI 控制器 */
	UPROPERTY(EditAnywhere, Category = Context)
	TObjectPtr<AAIController> Controller;

	/** 将要面向的 Actor */
	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<AActor> ActorToFaceTowards;
};

/**
 *  状态树任务，用于使 AI 控制的 Pawn 面向指定的 Actor
 */
USTRUCT(meta=(DisplayName="Face Towards Actor", Category="Shooter"))
struct FStateTreeFaceActorTask : public FStateTreeTaskCommonBase
{
	GENERATED_BODY()

	/* 确保我们使用的是正确的实例数据结构 */
	using FInstanceDataType = FStateTreeFaceActorInstanceData;
	virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }

	/** 在进入所属状态时运行 */
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;

	/** 在结束所属状态时运行 */
	virtual void ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;

#if WITH_EDITOR
	virtual FText GetDescription(const FGuid& ID, FStateTreeDataView InstanceDataView, const IStateTreeBindingLookup& BindingLookup, EStateTreeNodeFormatting Formatting = EStateTreeNodeFormatting::Text) const override;
#endif // WITH_EDITOR
};

////////////////////////////////////////////////////////////////////

/**
 *  面向位置状态树任务的实例数据结构
 */
USTRUCT()
struct FStateTreeFaceLocationInstanceData
{
	GENERATED_BODY()

	/** 用于决定焦点位置的 AI 控制器 */
	UPROPERTY(EditAnywhere, Category = Context)
	TObjectPtr<AAIController> Controller;

	/** 将要面向的位置 */
	UPROPERTY(EditAnywhere, Category = Parameter)
	FVector FaceLocation = FVector::ZeroVector;
};

/**
 *  状态树任务，用于使 AI 控制的 Pawn 面向世界中的某个位置
 */
USTRUCT(meta=(DisplayName="Face Towards Location", Category="Shooter"))
struct FStateTreeFaceLocationTask : public FStateTreeTaskCommonBase
{
	GENERATED_BODY()

	/* 确保我们使用的是正确的实例数据结构 */
	using FInstanceDataType = FStateTreeFaceLocationInstanceData;
	virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }

	/** 在进入所属状态时运行 */
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;

	/** 在结束所属状态时运行 */
	virtual void ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;

#if WITH_EDITOR
	virtual FText GetDescription(const FGuid& ID, FStateTreeDataView InstanceDataView, const IStateTreeBindingLookup& BindingLookup, EStateTreeNodeFormatting Formatting = EStateTreeNodeFormatting::Text) const override;
#endif // WITH_EDITOR
};

////////////////////////////////////////////////////////////////////

/**
 *  设置随机浮点值状态树任务的实例数据结构
 */
USTRUCT()
struct FStateTreeSetRandomFloatData
{
	GENERATED_BODY()

	/** 最小随机值 */
	UPROPERTY(EditAnywhere, Category = Parameter)
	float MinValue = 0.0f;

	/** 最大随机值 */
	UPROPERTY(EditAnywhere, Category = Parameter)
	float MaxValue = 0.0f;

	/** 输出计算出来的值 */
	UPROPERTY(EditAnywhere, Category = Output)
	float OutValue = 0.0f;
};

/**
 *  状态树任务，在指定范围内计算一个随机浮点值
 */
USTRUCT(meta=(DisplayName="Set Random Float", Category="Shooter"))
struct FStateTreeSetRandomFloatTask : public FStateTreeTaskCommonBase
{
	GENERATED_BODY()

	/* 确保我们使用的是正确的实例数据结构 */
	using FInstanceDataType = FStateTreeSetRandomFloatData;
	virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }

	/** 在进入所属状态时运行 */
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;

#if WITH_EDITOR
	virtual FText GetDescription(const FGuid& ID, FStateTreeDataView InstanceDataView, const IStateTreeBindingLookup& BindingLookup, EStateTreeNodeFormatting Formatting = EStateTreeNodeFormatting::Text) const override;
#endif // WITH_EDITOR
};

////////////////////////////////////////////////////////////////////

/**
 *  向目标射击状态树任务的实例数据结构
 */
USTRUCT()
struct FStateTreeShootAtTargetInstanceData
{
	GENERATED_BODY()

	/** 执行射击的 NPC */
	UPROPERTY(EditAnywhere, Category = Context)
	TObjectPtr<AShooterNPC> Character;

	/** 射击的目标 */
	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<AActor> Target;
};

/**
 *  状态树任务，使 NPC 向一个 Actor 射击
 */
USTRUCT(meta=(DisplayName="Shoot at Target", Category="Shooter"))
struct FStateTreeShootAtTargetTask : public FStateTreeTaskCommonBase
{
	GENERATED_BODY()

	/* 确保我们使用的是正确的实例数据结构 */
	using FInstanceDataType = FStateTreeShootAtTargetInstanceData;
	virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }

	/** 在进入所属状态时运行 */
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;

	/** 在结束所属状态时运行 */
	virtual void ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;

#if WITH_EDITOR
	virtual FText GetDescription(const FGuid& ID, FStateTreeDataView InstanceDataView, const IStateTreeBindingLookup& BindingLookup, EStateTreeNodeFormatting Formatting = EStateTreeNodeFormatting::Text) const override;
#endif // WITH_EDITOR
};

////////////////////////////////////////////////////////////////////

/**
 *  感知敌人状态树任务的实例数据结构
 */
USTRUCT()
struct FStateTreeSenseEnemiesInstanceData
{
	GENERATED_BODY()

	/** 感知中的 AI 控制器 */
	UPROPERTY(EditAnywhere, Category = Context)
	TObjectPtr<AShooterAIController> Controller;

	/** 感知中的 NPC */
	UPROPERTY(EditAnywhere, Category = Context)
	TObjectPtr<AShooterNPC> Character;

	/** 感知到的待瞄准 Actor */
	UPROPERTY(EditAnywhere, Category = Output)
	TObjectPtr<AActor> TargetActor;

	/** 感知到的待调查位置 */
	UPROPERTY(EditAnywhere, Category = Output)
	FVector InvestigateLocation = FVector::ZeroVector;

	/** 如果成功感知到目标则为 True */
	UPROPERTY(EditAnywhere, Category = Output)
	bool bHasTarget = false;

	/** 如果成功感知到调查位置则为 True */
	UPROPERTY(EditAnywhere, Category = Output)
	bool bHasInvestigateLocation = false;

	/** 感知 Actor 所需的标签 */
	UPROPERTY(EditAnywhere, Category = Parameter)
	FName SenseTag = FName("Player");

	/** 考虑为完全感知的视线锥半角 */
	UPROPERTY(EditAnywhere, Category = Parameter)
	float DirectLineOfSightCone = 85.0f;

	/** 上一次处理后的刺激强度 */
	UPROPERTY(EditAnywhere)
	float LastStimulusStrength = 0.0f;
};

/**
 *  状态树任务，使 NPC 处理 AI 感知并感应附近的敌人
 */
USTRUCT(meta=(DisplayName="Sense Enemies", Category="Shooter"))
struct FStateTreeSenseEnemiesTask : public FStateTreeTaskCommonBase
{
	GENERATED_BODY()

	/* 确保我们使用的是正确的实例数据结构 */
	using FInstanceDataType = FStateTreeSenseEnemiesInstanceData;
	virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }

	/** 在进入所属状态时运行 */
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;

	/** 在结束所属状态时运行 */
	virtual void ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;

#if WITH_EDITOR
	virtual FText GetDescription(const FGuid& ID, FStateTreeDataView InstanceDataView, const IStateTreeBindingLookup& BindingLookup, EStateTreeNodeFormatting Formatting = EStateTreeNodeFormatting::Text) const override;
#endif // WITH_EDITOR
};

////////////////////////////////////////////////////////////////////