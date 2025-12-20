#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ShooterWeaponHolder.generated.h"

class AShooterWeapon;
class UAnimMontage;


// 该类不需要修改。
UINTERFACE(MinimalAPI)
class UShooterWeaponHolder : public UInterface
{
	GENERATED_BODY()
};

/**
 *  射击游戏武器持有者类的通用接口
 */
class FPS_DEMO_API IShooterWeaponHolder
{
	GENERATED_BODY()

public:

	/** 将武器的网格体附加给所有者 */
	virtual void AttachWeaponMeshes(AShooterWeapon* Weapon) = 0;

	/** 为武器播放开火蒙太奇 */
	virtual void PlayFiringMontage(UAnimMontage* Montage) = 0;

	/** 对所有者应用武器后坐力 */
	virtual void AddWeaponRecoil(float Recoil) = 0;

	/** 使用当前弹药量更新武器 HUD */
	virtual void UpdateWeaponHUD(int32 CurrentAmmo, int32 MagazineSize) = 0;

	/** 计算并返回武器的瞄准位置 */
	virtual FVector GetWeaponTargetLocation() = 0;

	/** 将该类的武器授予所有者 */
	virtual void AddWeaponClass(const TSubclassOf<AShooterWeapon>& WeaponClass) = 0;

	/** 激活传入的武器 */
	virtual void OnWeaponActivated(AShooterWeapon* Weapon) = 0;

	/** 停用传入的武器 */
	virtual void OnWeaponDeactivated(AShooterWeapon* Weapon) = 0;

	/** 通知所有者武器冷却已结束，准备再次射击 */
	virtual void OnSemiWeaponRefire() = 0;
};
