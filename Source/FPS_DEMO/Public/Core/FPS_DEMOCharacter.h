#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "FPS_DEMOWeaponInterface.h"
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
class AFPS_DEMOCharacter : public ACharacter, public IFPS_DEMOWeaponInterface
{
	GENERATED_BODY()

	/** Pawn 网格体：第一人称视图（手臂；仅自己可见） */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* FirstPersonMesh;

	/** 第一人称相机 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

protected:
	/** 当前装备并准备射击的武器 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapons")
	class AFPS_DEMOWeapon* EquippedWeapon;

	/** 角色拾取的武器列表 */
	UPROPERTY(BlueprintReadOnly, Category = "Weapons")
	TArray<class AFPS_DEMOWeapon*> InventoryWeapons;

	/** 第一人称网格体武器插槽名称 */
	UPROPERTY(EditAnywhere, Category = "Weapons")
	FName FirstPersonWeaponSocket = FName("HandGrip_R");

	/** 第三人称网格体武器插槽名称 */
	UPROPERTY(EditAnywhere, Category = "Weapons")
	FName ThirdPersonWeaponSocket = FName("HandGrip_R");

protected:
	/** 检查是否已经拥有该类型的武器 */
	class AFPS_DEMOWeapon* FindWeaponOfType(TSubclassOf<class AFPS_DEMOWeapon> WeaponClass) const;

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

	/** 开火输入动作 */
	UPROPERTY(EditAnywhere, Category ="Input")
	class UInputAction* WeaponFireAction;
	
public:
	AFPS_DEMOCharacter();
	/** 实现：将武器附加到角色模型上 */
	virtual void AttachWeaponMeshes(class AFPS_DEMOWeapon* Weapon) override;
	/** 实现：播放角色的开火动画 */
	virtual void PlayFiringMontage(class UAnimMontage* Montage) override;
	/** 实现：角色对武器后坐力的反应 */
	virtual void AddWeaponRecoil(float Recoil) override;
	/** 实现：更新 HUD 弹药显示 */
	virtual void UpdateWeaponHUD(int32 CurrentAmmo, int32 MagazineSize) override;
	/** 实现：返回相机或准星指向的位置 */
	virtual FVector GetWeaponTargetLocation() override;
	/** 实现：添加新的武器类型 */
	virtual void AddWeaponClass(const TSubclassOf<class AFPS_DEMOWeapon>& WeaponClass) override;
	/** 实现：武器激活时的逻辑（如通知 UI） */
	virtual void OnWeaponActivated(class AFPS_DEMOWeapon* Weapon) override;
	/** 实现：武器停用逻辑 */
	virtual void OnWeaponDeactivated(class AFPS_DEMOWeapon* Weapon) override;
	/** 实现：半自动武器连发逻辑 */
	virtual void OnSemiWeaponRefire() override;

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

	/** 处理开火开始输入 */
	UFUNCTION(BlueprintCallable, Category="Input")
	void FireWeapon();

	/** 处理开火结束输入 */
	UFUNCTION(BlueprintCallable, Category="Input")
	void StopFireWeapon();

protected:

	/** 设置输入动作绑定 */
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	

public:

	/** 返回第一人称网格体 **/
	USkeletalMeshComponent* GetFirstPersonMesh() const { return FirstPersonMesh; }

	/** 返回第一人称相机组件 **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }
};

