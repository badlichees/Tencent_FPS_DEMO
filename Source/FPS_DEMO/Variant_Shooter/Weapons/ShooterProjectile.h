#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShooterProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class ACharacter;
class UPrimitiveComponent;

/**
 *  用于第一人称射击游戏的简单弹丸类
 */
UCLASS(abstract)
class FPS_DEMO_API AShooterProjectile : public AActor
{
	GENERATED_BODY()
	
	/** 为弹丸提供碰撞检测 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	USphereComponent* CollisionComponent;

	/** 处理弹丸的移动 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovement;

protected:

	/** 此弹丸命中时产生的 AI 感知噪声的音量 */
	UPROPERTY(EditAnywhere, Category="Projectile|Noise", meta = (ClampMin = 0, ClampMax = 100))
	float NoiseLoudness = 3.0f;

	/** 此弹丸命中时产生的 AI 感知噪声的范围 */
	UPROPERTY(EditAnywhere, Category="Projectile|Noise", meta = (ClampMin = 0, ClampMax = 100000, Units = "cm"))
	float NoiseRange = 3000.0f;

	/** 此弹丸命中时产生的 AI 感知噪声的标签 */
	UPROPERTY(EditAnywhere, Category="Noise")
	FName NoiseTag = FName("Projectile");

	/** 命中时应用的物理力 */
	UPROPERTY(EditAnywhere, Category="Projectile|Hit", meta = (ClampMin = 0, ClampMax = 50000))
	float PhysicsForce = 100.0f;

	/** 命中时应用的伤害 */
	UPROPERTY(EditAnywhere, Category="Projectile|Hit", meta = (ClampMin = 0, ClampMax = 100))
	float HitDamage = 25.0f;

	/** 应用的伤害类型。可用于表示特定类型的伤害，如火焰、爆炸等。 */
	UPROPERTY(EditAnywhere, Category="Projectile|Hit")
	TSubclassOf<UDamageType> HitDamageType;

	/** 如果为 true，弹丸可以伤害射击它的角色 */
	UPROPERTY(EditAnywhere, Category="Projectile|Hit")
	bool bDamageOwner = false;

	/** 如果为 true，弹丸会爆炸并对范围内的所有 Actor 应用径向伤害 */
	UPROPERTY(EditAnywhere, Category="Projectile|Explosion")
	bool bExplodeOnHit = false;

	/** 受爆炸伤害影响的 Actor 的最大距离 */
	UPROPERTY(EditAnywhere, Category="Projectile|Explosion", meta = (ClampMin = 0, ClampMax = 5000, Units = "cm"))
	float ExplosionRadius = 500.0f;	

	/** 如果为 true，此弹物已经命中了另一个表面 */
	bool bHit = false;

	/** 命中后等待多久再销毁此弹丸 */
	UPROPERTY(EditAnywhere, Category="Projectile|Destruction", meta = (ClampMin = 0, ClampMax = 10, Units = "s"))
	float DeferredDestructionTime = 5.0f;

	/** 用于处理此弹丸延迟销毁的计时器 */
	FTimerHandle DestructionTimer;

public:	

	/** 构造函数 */
	AShooterProjectile();

protected:
	
	/** 游戏开始初始化 */
	virtual void BeginPlay() override;

	/** 游戏结束清理 */
	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;

	/** 处理碰撞 */
	virtual void NotifyHit(class UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;

protected:

	/** 查找爆炸半径内的 Actor 并对其造成伤害 */
	void ExplosionCheck(const FVector& ExplosionCenter);

	/** 为给定的 Actor 处理弹丸命中 */
	void ProcessHit(AActor* HitActor, UPrimitiveComponent* HitComp, const FVector& HitLocation, const FVector& HitDirection);

	/** 将控制权交给蓝图以实现在命中时的任何效果。 */
	UFUNCTION(BlueprintImplementableEvent, Category="Projectile", meta = (DisplayName = "On Projectile Hit"))
	void BP_OnProjectileHit(const FHitResult& Hit);

	/** 从销毁计时器调用以销毁此弹丸 */
	void OnDeferredDestruction();

};
