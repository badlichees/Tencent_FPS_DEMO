// Fill out your copyright notice in the Description page of Project Settings.

#include "MyFPSHUD.h"
#include "Engine/Canvas.h"
#include "Engine/Texture2D.h"
#include "CanvasItem.h"

AMyFPSHUD::AMyFPSHUD()
{
}

void AMyFPSHUD::BeginPlay()
{
	Super::BeginPlay();
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
