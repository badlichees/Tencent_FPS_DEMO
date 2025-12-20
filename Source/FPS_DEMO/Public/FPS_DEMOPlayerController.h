#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "FPS_DEMOPlayerController.generated.h"

class UInputMappingContext;
class UUserWidget;

/**
 *  基础第一人称 Player Controller（玩家控制器）
 *  管理输入映射上下文（Input Mapping Context）。
 *  重写了 Player Camera Manager（玩家相机管理器）类。
 */
UCLASS(abstract, config="Game")
class FPS_DEMO_API AFPS_DEMOPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:

	/** 构造函数 */
	AFPS_DEMOPlayerController();

protected:

	/** 输入映射上下文 */
	UPROPERTY(EditAnywhere, Category="Input|Input Mappings")
	TArray<UInputMappingContext*> DefaultMappingContexts;

	/** 输入映射上下文 */
	UPROPERTY(EditAnywhere, Category="Input|Input Mappings")
	TArray<UInputMappingContext*> MobileExcludedMappingContexts;

	/** 要生成的移动端控制微件（Widget） */
	UPROPERTY(EditAnywhere, Category="Input|Touch Controls")
	TSubclassOf<UUserWidget> MobileControlsWidgetClass;

	/** 指向移动端控制微件的指针 */
	UPROPERTY()
	TObjectPtr<UUserWidget> MobileControlsWidget;

	/** 如果为 true，即使不在移动平台上运行，玩家也将强制使用 UMG 触摸控制 */
	UPROPERTY(EditAnywhere, Config, Category = "Input|Touch Controls")
	bool bForceTouchControls = false;

	/** 游戏开始初始化 */
	virtual void BeginPlay() override;

	/** 输入映射上下文设置 */
	virtual void SetupInputComponent() override;

	/** 如果玩家应该使用 UMG 触摸控制，则返回 true */
	bool ShouldUseTouchControls() const;
};
