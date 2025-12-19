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
	
	/** 重写 BeginPlay 以在服务器生成武器 */
	virtual void BeginPlay() override;

	/** 执行移动 (父类已声明为 BlueprintCallable) */
	virtual void DoMove(float Right, float Forward);

	/** 执行瞄准转向 (父类已声明为 BlueprintCallable) */
	virtual void DoAim(float Yaw, float Pitch) override;

	/** 执行跳跃 (父类已声明为 BlueprintCallable) */
	virtual void DoJumpStart();
	virtual void DoJumpEnd();

	/** 执行开火请求 (子类新增接口) */
	UFUNCTION(BlueprintCallable, Category = "Combat")
	virtual void DoFire();

protected:
	/** 默认武器类 */
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<class AMyWeapon> DefaultWeaponClass;

	/** 当前装备的武器 (同步变量) */
	UPROPERTY(Replicated)
	class AMyWeapon* CurrentWeapon;

	/** 武器挂载的插槽名称 */
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	FName WeaponSocketName = TEXT("GripPoint");

	/** Fire Input Action */
	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* FireAction;

	/** 鼠标灵敏度 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	float MouseSensitivity = 1.0f;

	/** 权威服务器开火逻辑 */
	UFUNCTION(Server, Reliable)
	void ServerDoFire();

	/** [动画同步] 角色当前移动速度 */

	/** [动画同步] 角色当前移动速度 */
	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	float MovementSpeed;

	/** [动画同步] 角色是否正在空中（跳跃/坠落） */
	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	bool bIsFalling;

	virtual void Tick(float DeltaTime) override;

	/** 绑定输入 */
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};