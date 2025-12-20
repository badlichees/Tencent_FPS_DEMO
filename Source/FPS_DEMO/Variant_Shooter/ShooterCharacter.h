#pragma once

#include "CoreMinimal.h"
#include "FPS_DEMOCharacter.h"
#include "ShooterWeaponHolder.h"
#include "ShooterCharacter.generated.h"

class AShooterWeapon;
class UInputAction;
class UInputComponent;
class UPawnNoiseEmitterComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FBulletCountUpdatedDelegate, int32, MagazineSize, int32, Bullets);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDamagedDelegate, float, LifePercent);

/**
 *  玩家可控制的第一人称射击角色
 *  通过 IShooterWeaponHolder 接口管理武器库存
 *  管理生命值和死亡逻辑
 */
UCLASS(abstract)
class FPS_DEMO_API AShooterCharacter : public AFPS_DEMOCharacter, public IShooterWeaponHolder
{
	GENERATED_BODY()
	
	/** AI 噪声发射器组件 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	UPawnNoiseEmitterComponent* PawnNoiseEmitter;

protected:

	/** 开火武器输入动作 */
	UPROPERTY(EditAnywhere, Category ="Input")
	UInputAction* FireAction;

	/** 切换武器输入动作 */
	UPROPERTY(EditAnywhere, Category ="Input")
	UInputAction* SwitchWeaponAction;

	/** 第一人称网格体武器插槽名称 */
	UPROPERTY(EditAnywhere, Category ="Weapons")
	FName FirstPersonWeaponSocket = FName("HandGrip_R");

	/** 第三人称网格体武器插槽名称 */
	UPROPERTY(EditAnywhere, Category ="Weapons")
	FName ThirdPersonWeaponSocket = FName("HandGrip_R");

	/** 用于瞄准追踪的最大距离 */
	UPROPERTY(EditAnywhere, Category ="Aim", meta = (ClampMin = 0, ClampMax = 100000, Units = "cm"))
	float MaxAimDistance = 10000.0f;

	/** 该角色可以拥有的最大生命值（HP） */
	UPROPERTY(EditAnywhere, Category="Health")
	float MaxHP = 500.0f;

	/** 该角色剩余的当前生命值（HP） */
	float CurrentHP = 0.0f;

	/** 该角色的阵营 ID */
	UPROPERTY(EditAnywhere, Category="Team")
	uint8 TeamByte = 0;

	/** 角色死亡时赋予的 Actor 标签 */
	UPROPERTY(EditAnywhere, Category="Team")
	FName DeathTag = FName("Dead");

	/** 角色拾取的武器列表 */
	TArray<AShooterWeapon*> OwnedWeapons;

	/** 当前装备并准备射击的武器 */
	TObjectPtr<AShooterWeapon> CurrentWeapon;

	UPROPERTY(EditAnywhere, Category ="Destruction", meta = (ClampMin = 0, ClampMax = 10, Units = "s"))
	float RespawnTime = 5.0f;

	FTimerHandle RespawnTimer;

public:

	/** 子弹计数更新委托 */
	FBulletCountUpdatedDelegate OnBulletCountUpdated;

	/** 受伤委托 */
	FDamagedDelegate OnDamaged;

public:

	/** 构造函数 */
	AShooterCharacter();

protected:

	/** 游戏开始初始化 */
	virtual void BeginPlay() override;

	/** 游戏结束清理 */
	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;

	/** 设置输入动作绑定 */
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

public:

	/** 处理受到的伤害 */
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

public:

	/** 处理来自控制器或 UI 界面的瞄准输入 */
	virtual void DoAim(float Yaw, float Pitch) override;

	/** 处理来自控制器或 UI 界面的移动输入 */
	virtual void DoMove(float Right, float Forward)  override;

	/** 处理来自控制器或 UI 界面的跳跃开始输入 */
	virtual void DoJumpStart()  override;

	/** 处理来自控制器或 UI 界面的跳跃结束输入 */
	virtual void DoJumpEnd()  override;

	/** 处理开始开火输入 */
	UFUNCTION(BlueprintCallable, Category="Input")
	void DoStartFiring();

	/** 处理停止开火输入 */
	UFUNCTION(BlueprintCallable, Category="Input")
	void DoStopFiring();

	/** 处理切换武器输入 */
	UFUNCTION(BlueprintCallable, Category="Input")
	void DoSwitchWeapon();

public:

	//~开始 IShooterWeaponHolder 接口实现

	/** 将武器的网格体附加到所有者 */
	virtual void AttachWeaponMeshes(AShooterWeapon* Weapon) override;

	/** 为武器播放开火蒙版动画 */
	virtual void PlayFiringMontage(UAnimMontage* Montage) override;

	/** 对所有者应用武器后坐力 */
	virtual void AddWeaponRecoil(float Recoil) override;

	/** 使用当前弹药计数更新武器 UI */
	virtual void UpdateWeaponHUD(int32 CurrentAmmo, int32 MagazineSize) override;

	/** 计算并返回武器的瞄准位置 */
	virtual FVector GetWeaponTargetLocation() override;

	/** 将该类的武器赠予所有者 */
	virtual void AddWeaponClass(const TSubclassOf<AShooterWeapon>& WeaponClass) override;

	/** 激活传入的武器 */
	virtual void OnWeaponActivated(AShooterWeapon* Weapon) override;

	/** 停用传入的武器 */
	virtual void OnWeaponDeactivated(AShooterWeapon* Weapon) override;

	/** 通知所有者武器冷却已过期，可以再次射击 */
	virtual void OnSemiWeaponRefire() override;

	//~结束 IShooterWeaponHolder 接口实现

protected:

	/** 如果角色已经拥有给定类的武器，则返回 true */
	AShooterWeapon* FindWeaponOfType(TSubclassOf<AShooterWeapon> WeaponClass) const;

	/** 当此角色的生命值（HP）耗尽时调用 */
	void Die();

	/** 调用以允许蓝图代码对该角色的死亡做出反应 */
	UFUNCTION(BlueprintImplementableEvent, Category="Shooter", meta = (DisplayName = "On Death"))
	void BP_OnDeath();

	/** 从重生计时器调用，以销毁此角色并强制玩家控制器重新生成 */
	void OnRespawn();

public:

	/** 如果角色已死亡则返回 true */
	bool IsDead() const;
};
