#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FPS_DEMOWeapon.generated.h"

class USkeletalMeshComponent;
class IFPS_DEMOWeaponInterface;
class UNiagaraSystem;
class USoundBase;

/**
 * 武器基类
 */
UCLASS(abstract)
class FPS_DEMO_API AFPS_DEMOWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	AFPS_DEMOWeapon();

	/** 第一人称下的视觉模型 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USkeletalMeshComponent* WeaponMeshFP;

	/** 第三人称（或其他玩家看到）的视觉模型 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USkeletalMeshComponent* WeaponMeshTP;

	/** 开始开火 */
	virtual void StartFiring();

	/** 停止开火 */
	virtual void StopFiring();

	/** 激活武器 */
	virtual void ActivateWeapon();

	/** 停用武器 */
	virtual void DeactivateWeapon();

protected:
	virtual void BeginPlay() override;

	/** 执行单次射击 */
	virtual void FireOnce();

	/** 拥有此武器的接口持有人 */
	IFPS_DEMOWeaponInterface* WeaponOwner;

	//~ 射击参数
	/** 枪口插槽名称（用于特效生成和射线起点） */
	UPROPERTY(EditAnywhere, Category = "Firing")
	FName MuzzleSocketName = FName("Muzzle");

	/** 枪口闪光特效 (Niagara) */
	UPROPERTY(EditAnywhere, Category = "Firing|Effects")
	UNiagaraSystem* MuzzleFlashEffect;

	/** 击中特效 (Niagara) */
	UPROPERTY(EditAnywhere, Category = "Firing|Effects")
	UNiagaraSystem* ImpactEffect;

	/** 开火音效 */
	UPROPERTY(EditAnywhere, Category = "Firing|Effects")
	USoundBase* FireSound;

	/** 每次射击的伤害值 */
	UPROPERTY(EditAnywhere, Category = "Firing", meta = (ClampMin = 0))
	float Damage = 25.0f;

	/** 射程（厘米） */
	UPROPERTY(EditAnywhere, Category = "Firing", meta = (ClampMin = 0, Units = "cm"))
	float MaxRange = 10000.0f;

	/** 是否为全自动武器 */
	UPROPERTY(EditAnywhere, Category = "Firing")
	bool bIsAutomatic = true;

	/** 射速（每分钟发射数） */
	UPROPERTY(EditAnywhere, Category = "Firing", meta = (ClampMin = 1, EditCondition = "bIsAutomatic"))
	float FireRate = 600.0f;

	/** 是否正在开火 */
	bool bIsFiring = false;

	/** 自动开火计时器 */
	FTimerHandle FireTimerHandle;
};
