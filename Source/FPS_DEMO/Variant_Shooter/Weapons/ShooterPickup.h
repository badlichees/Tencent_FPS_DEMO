#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "Engine/StaticMesh.h"
#include "ShooterPickup.generated.h"

class USphereComponent;
class UPrimitiveComponent;
class AShooterWeapon;

/**
 *  自建结构体，继承自UE的FTableRowBase，包含有关武器拾取类型的信息
 */
USTRUCT(BlueprintType)
struct FWeaponTableRow : public FTableRowBase
{
	GENERATED_BODY()

	/** 要在拾取物上显示的网格体 */
	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UStaticMesh> StaticMesh;

	/** 拾取时授予的武器类 */
	UPROPERTY(EditAnywhere)
	TSubclassOf<AShooterWeapon> WeaponToSpawn;
};

/**
 *  简单的射击游戏武器拾取物
 */
UCLASS(abstract)
class FPS_DEMO_API AShooterPickup : public AActor
{
	GENERATED_BODY()

	/** 碰撞球体 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	USphereComponent* SphereCollision;

	/** 武器拾取网格体。其网格体资源从武器数据表中设置 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Mesh;
	
protected:

	/** 拾取的武器类型及其视觉效果的数据 */
	UPROPERTY(EditAnywhere, Category="Pickup")
	FDataTableRowHandle WeaponType;

	/** 拾取时授予的武器类型。从武器数据表中设置 */
	TSubclassOf<AShooterWeapon> WeaponClass;
	
	/** 重生此拾取物前的等待时间 */
	UPROPERTY(EditAnywhere, Category="Pickup", meta = (ClampMin = 0, ClampMax = 120, Units = "s"))
	float RespawnTime = 4.0f;

	/** 重生拾取物的计时器 */
	FTimerHandle RespawnTimer;

public:	
	
	/** 构造函数 */
	AShooterPickup();

protected:

	/** 原生构造脚本 */
	virtual void OnConstruction(const FTransform& Transform) override;

	/** 游戏开始初始化 */
	virtual void BeginPlay() override;

	/** 游戏结束清理 */
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	/** 处理碰撞重叠 */
	UFUNCTION()
	virtual void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:

	/** 当需要重生此拾取物时调用 */
	void RespawnPickup();

	/** 将控制权交给蓝图以动画化拾取物重生。应通过调用 FinishRespawn 结束 */
	UFUNCTION(BlueprintImplementableEvent, Category="Pickup", meta = (DisplayName = "OnRespawn"))
	void BP_OnRespawn();

	/** 重生后启用此拾取物 */
	UFUNCTION(BlueprintCallable, Category="Pickup")
	void FinishRespawn();
};
