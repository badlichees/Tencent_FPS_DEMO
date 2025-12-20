#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "FPS_DEMOCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

/**
 *  基础第一人称角色
 */
UCLASS(abstract)
class AFPS_DEMOCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Pawn 网格体：第一人称视图（手臂；仅自己可见） */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* FirstPersonMesh;

	/** 第一人称相机 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

protected:

	/** 跳跃输入动作 */
	UPROPERTY(EditAnywhere, Category ="Input")
	UInputAction* JumpAction;

	/** 移动输入动作 */
	UPROPERTY(EditAnywhere, Category ="Input")
	UInputAction* MoveAction;

	/** 查看输入动作 */
	UPROPERTY(EditAnywhere, Category ="Input")
	class UInputAction* LookAction;

	/** 鼠标查看输入动作 */
	UPROPERTY(EditAnywhere, Category ="Input")
	class UInputAction* MouseLookAction;
	
public:
	AFPS_DEMOCharacter();

protected:

	/** 当触发移动输入时调用 */
	void MoveInput(const FInputActionValue& Value);

	/** 当触发查看输入时调用 */
	void LookInput(const FInputActionValue& Value);

	/** 处理来自控制器或 UI 界面的瞄准输入 */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoAim(float Yaw, float Pitch);

	/** 处理来自控制器或 UI 界面的移动输入 */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoMove(float Right, float Forward);

	/** 处理来自控制器或 UI 界面的跳跃开始输入 */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpStart();

	/** 处理来自控制器或 UI 界面的跳跃结束输入 */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpEnd();

protected:

	/** 设置输入动作绑定 */
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	

public:

	/** 返回第一人称网格体 **/
	USkeletalMeshComponent* GetFirstPersonMesh() const { return FirstPersonMesh; }

	/** 返回第一人称相机组件 **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

};

