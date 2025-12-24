#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "FPS_DEMOPickup.generated.h"

class USphereComponent;
class AFPS_DEMOWeapon;

/**
 * 武器拾取数据结构
 */
USTRUCT(BlueprintType)
struct FFPS_DEMOWeaponTableRow : public FTableRowBase
{
	GENERATED_BODY()

	/** 拾取物显示的静态网格体 */
	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UStaticMesh> StaticMesh;

	/** 拾取时获得的武器类 */
	UPROPERTY(EditAnywhere)
	TSubclassOf<AFPS_DEMOWeapon> WeaponToSpawn;
};

/**
 * FPS_DEMO 武器拾取物
 */
UCLASS()
class FPS_DEMO_API AFPS_DEMOPickup : public AActor
{
	GENERATED_BODY()
	
public:	
	AFPS_DEMOPickup();

protected:
	virtual void BeginPlay() override;

	/** 响应重叠事件 */
	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/** 重生逻辑 */
	void RespawnPickup();

	/** 重生结束，恢复碰撞 */
	UFUNCTION(BlueprintCallable, Category = "Pickup")
	void FinishRespawn();

	/** 给蓝图使用的重生表现事件 */
	UFUNCTION(BlueprintImplementableEvent, Category = "Pickup")
	void BP_OnRespawn();

protected:
	/** 碰撞检测球体 */
	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent* SphereCollision;

	/** 视觉模型 */
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* PickupMesh;

	/** 绑定的武器数据（从数据表选择） */
	UPROPERTY(EditAnywhere, Category = "Pickup")
	FDataTableRowHandle WeaponType;

	/** 缓存的武器类 */
	TSubclassOf<AFPS_DEMOWeapon> WeaponClass;

	/** 重生时间 */
	UPROPERTY(EditAnywhere, Category = "Pickup")
	float RespawnTime = 5.0f;

	FTimerHandle RespawnTimer;

public:
	virtual void OnConstruction(const FTransform& Transform) override;
};
