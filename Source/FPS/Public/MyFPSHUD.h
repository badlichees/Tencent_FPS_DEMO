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

	UPROPERTY(EditDefaultsOnly, Category = "Crosshair")
	class UTexture2D* CrosshairTexture;
};
