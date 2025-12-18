#pragma once

#include "CoreMinimal.h"
#include "FPSCharacter.h"
#include "MyFPSCharacter.generated.h"

/**
 * AMyFPSCharacter
 * 自定义玩家角色类，继承自模版提供的 AFPSCharacter。
 * 这样我们可以保留模版功能（如移动逻辑），同时在这里添加我们自己的功能（如战斗、得分）。
 */
UCLASS()
class FPS_API AMyFPSCharacter : public AFPSCharacter
{
	GENERATED_BODY()

public:
	AMyFPSCharacter();

protected:
	/** Fire Input Action */
	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* FireAction;

	/** 射程 */
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	float WeaponRange = 5000.0f;

	/** 鼠标灵敏度 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	float MouseSensitivity = 1.0f;

	/** 攻击逻辑 */
	virtual void DoFire();

	/** 重写转向逻辑以应用灵敏度 */
	virtual void DoAim(float Yaw, float Pitch) override;

	/** 绑定输入 */
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};