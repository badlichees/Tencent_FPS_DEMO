#include "FPS_DEMOPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "InputMappingContext.h"
#include "FPS_DEMOCameraManager.h"
#include "Blueprint/UserWidget.h"
#include "FPS_DEMO.h"
#include "Widgets/Input/SVirtualJoystick.h"

AFPS_DEMOPlayerController::AFPS_DEMOPlayerController()
{
	// 设置玩家相机管理器类，因为控制器需要管理相机
	PlayerCameraManagerClass = AFPS_DEMOCameraManager::StaticClass();
}

// 在开始时会先检查是否为移动端，然后生成触摸控制微件
void AFPS_DEMOPlayerController::BeginPlay()
{
	Super::BeginPlay();

	
	// 仅在本地玩家控制器上生成触摸控制
	if (ShouldUseTouchControls() && IsLocalPlayerController())
	{
		// 生成移动端控制微件
		MobileControlsWidget = CreateWidget<UUserWidget>(this, MobileControlsWidgetClass);

		if (MobileControlsWidget)
		{
			// 将控制组件添加到玩家屏幕
			MobileControlsWidget->AddToPlayerScreen(0);

		} 
		else 
		{

			UE_LOG(LogMyDEMO, Error, TEXT("无法生成移动端控制微件。"));

		}

	}
}

// 设置输入组件，用于处理玩家输入
void AFPS_DEMOPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// 仅为本地玩家控制器添加 IMC（输入控制方案）
	if (IsLocalPlayerController())
	{
		// 添加输入映射上下文 (Input Mapping Context)
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			for (UInputMappingContext* CurrentContext : DefaultMappingContexts)
			{
				Subsystem->AddMappingContext(CurrentContext, 0);
			}

			// 仅在不使用移动端触摸输入时添加这些 IMC
			if (!ShouldUseTouchControls())
			{
				for (UInputMappingContext* CurrentContext : MobileExcludedMappingContexts)
				{
					Subsystem->AddMappingContext(CurrentContext, 0);
				}
			}
		}
	}
	
}

// 尾部const表示这个函数不会修改类的任何成员变量
bool AFPS_DEMOPlayerController::ShouldUseTouchControls() const
{
	// 我们是否在移动平台上？是否强制开启触摸？
	return SVirtualJoystick::ShouldDisplayTouchInterface() || bForceTouchControls;
}
