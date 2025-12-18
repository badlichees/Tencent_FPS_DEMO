#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NiagaraSystem.h"
#include "MyWeapon.generated.h"

UCLASS()
class FPS_API AMyWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	AMyWeapon();

protected:
	virtual void BeginPlay() override;

public:
	/** 武器网格体 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USkeletalMeshComponent* MeshComp;

	/** 枪口火光特效 (Niagara) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	class UNiagaraSystem* MuzzleFlash;

	/** 开火音效 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	class USoundBase* FireSound;

	/** 射程 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float WeaponRange = 5000.0f;

	/** 武器持枪姿态叠加层 (Niagara 动画层) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	TSubclassOf<class UAnimInstance> WeaponAnimLayer;

	/** 核心射击函数，由 Character 调用 */
	virtual void Fire(APawn* Shooter);

	/** 在所有客户端播放开火特效 */
	UFUNCTION(NetMulticast, Unreliable)
	void Multi_PlayFireEffects();

};
