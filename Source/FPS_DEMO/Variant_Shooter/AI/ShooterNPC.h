#pragma once

#include "CoreMinimal.h"
#include "FPS_DEMOCharacter.h"
#include "ShooterWeaponHolder.h"
#include "ShooterNPC.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPawnDeathDelegate);

class AShooterWeapon;

/**
 *  一个简单的 AI 控制的射击游戏 NPC
 *  通过其 AI 控制器管理的状态树（StateTree）执行行为
 *  持有并管理武器
 */
UCLASS(abstract)
class FPS_DEMO_API AShooterNPC : public AFPS_DEMOCharacter, public IShooterWeaponHolder
{
	GENERATED_BODY()

public:

	/** 该角色的当前 HP。如果通过伤害降至零，则角色死亡 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Damage")
	float CurrentHP = 100.0f;

protected:

	/** 死亡时进入布娃娃状态所使用的碰撞配置名称 */
	UPROPERTY(EditAnywhere, Category="Damage")
	FName RagdollCollisionProfile = FName("Ragdoll");

	/** 死亡后等待多久再销毁此 Actor */
	UPROPERTY(EditAnywhere, Category="Damage")
	float DeferredDestructionTime = 5.0f;

	/** 该角色的阵营字节 */
	UPROPERTY(EditAnywhere, Category="Team")
	uint8 TeamByte = 1;

	/** 该角色死亡时授予的 Actor 标签 */
	UPROPERTY(EditAnywhere, Category="Team")
	FName DeathTag = FName("Dead");

	/** 指向装备武器的指针 */
	TObjectPtr<AShooterWeapon> Weapon;

	/** 为该角色生成的武器类型 */
	UPROPERTY(EditAnywhere, Category="Weapon")
	TSubclassOf<AShooterWeapon> WeaponClass;

	/** 瞄准计算的最大范围 */
	UPROPERTY(EditAnywhere, Category="Aim")
	float AimRange = 10000.0f;

	/** 瞄准时应用的锥形偏差 */
	UPROPERTY(EditAnywhere, Category="Aim")
	float AimVarianceHalfAngle = 10.0f;

	/** 瞄准时应用的距离目标中心最小的垂直偏移 */
	UPROPERTY(EditAnywhere, Category="Aim")
	float MinAimOffsetZ = -35.0f;

	/** 瞄准时应用的距离目标中心最大的垂直偏移 */
	UPROPERTY(EditAnywhere, Category="Aim")
	float MaxAimOffsetZ = -60.0f;

	/** 当前瞄准的目标 Actor */
	TObjectPtr<AActor> CurrentAimTarget;

	/** 如果为 true，该角色当前正在发射武器 */
	bool bIsShooting = false;

	/** 如果为 true，该角色已经死亡 */
	bool bIsDead = false;

	/** 死亡后的延迟销毁计时器 */
	FTimerHandle DeathTimer;

public:

	/** 当此 NPC 死亡时调用的委托 */
	FPawnDeathDelegate OnPawnDeath;

protected:

	/** 游戏开始初始化 */
	virtual void BeginPlay() override;

	/** 游戏结束清理 */
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:

	/** 处理受到的伤害 */
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

public:

	//~Begin IShooterWeaponHolder 接口

	/** 将武器的网格体附加给所有者 */
	virtual void AttachWeaponMeshes(AShooterWeapon* Weapon) override;

	/** 为武器播放开火蒙太奇 */
	virtual void PlayFiringMontage(UAnimMontage* Montage) override;

	/** 对所有者应用武器后坐力 */
	virtual void AddWeaponRecoil(float Recoil) override;

	/** 使用当前弹药量更新武器 HUD */
	virtual void UpdateWeaponHUD(int32 CurrentAmmo, int32 MagazineSize) override;

	/** 计算并返回武器的瞄准位置 */
	virtual FVector GetWeaponTargetLocation() override;

	/** 将该类的武器授予所有者 */
	virtual void AddWeaponClass(const TSubclassOf<AShooterWeapon>& WeaponClass) override;

	/** 激活传入的武器 */
	virtual void OnWeaponActivated(AShooterWeapon* Weapon) override;

	/** 停用传入的武器 */
	virtual void OnWeaponDeactivated(AShooterWeapon* Weapon) override;

	/** 通知所有者武器冷却已结束，准备再次射击 */
	virtual void OnSemiWeaponRefire() override;

	//~End IShooterWeaponHolder 接口

protected:

	/** 当 HP 耗尽且角色应该死亡时调用 */
	void Die();

	/** 死亡后调用以销毁 Actor */
	void DeferredDestruction();

public:

	/** 通知此角色开始向传入的 Actor 射击 */
	void StartShooting(AActor* ActorToShoot);

	/** 通知此角色停止射击 */
	void StopShooting();
};
