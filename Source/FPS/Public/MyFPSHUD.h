#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MyFPSHUD.generated.h"

/**
 * AMyFPSHUD
 * 自定义 HUD 类。
 */
UCLASS()
class FPS_API AMyFPSHUD : public AHUD
{
	GENERATED_BODY()

public:
	AMyFPSHUD();

	virtual void DrawHUD() override;

protected:
	virtual void BeginPlay() override;

	/** 准星纹理 */
	UPROPERTY(EditDefaultsOnly, Category = "MyFPS Base | UI")
	class UTexture2D* CrosshairTexture;

	/**
	 * 主 HUD 控件类 (在蓝图中把 WBP_HUD 填在这里)
	 * 使用 TSubclassOf<UUserWidget> 来限制只能选 UserWidget 类型
	 */
	UPROPERTY(EditDefaultsOnly, Category = "MyFPS Base | UI")
	TSubclassOf<class UUserWidget> MainHUDWidgetClass;

private:
	/** 实际生成的 UserWidget 实例指针 */
	UPROPERTY()
	class UUserWidget* MainHUDWidget;
};