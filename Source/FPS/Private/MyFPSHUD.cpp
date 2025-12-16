#include "MyFPSHUD.h"
#include "Engine/Canvas.h"
#include "Engine/Texture2D.h"
#include "CanvasItem.h"
#include "Blueprint/UserWidget.h"

AMyFPSHUD::AMyFPSHUD()
{
}

void AMyFPSHUD::BeginPlay()
{
	Super::BeginPlay();

	// 如果我们在蓝图里配置了 Widget 类，就在这里创建它
	if (MainHUDWidgetClass)
	{
		// 1. 创建 Widget
		MainHUDWidget = CreateWidget<UUserWidget>(GetWorld(), MainHUDWidgetClass);
		
		// 2. 添加到视口（Viewport）
		if (MainHUDWidget)
		{
			MainHUDWidget->AddToViewport();
		}
	}
}

void AMyFPSHUD::DrawHUD()
{
	Super::DrawHUD();

	if (CrosshairTexture)
	{
		const FVector2D Center(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);
		
		// 简单的绘制纹理方法
		DrawTextureSimple(
			CrosshairTexture, 
			Center.X - (CrosshairTexture->GetSurfaceWidth() * 0.5f), 
			Center.Y - (CrosshairTexture->GetSurfaceHeight() * 0.5f), 
			1.0f, 
			true // 半透明混合
		);
	}
}
