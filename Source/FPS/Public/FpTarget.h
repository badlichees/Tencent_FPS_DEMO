#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FpTarget.generated.h"

UCLASS(Blueprintable)
class FPS_API AFpTarget : public AActor
{
	GENERATED_BODY()
	
public:	
	AFpTarget();

	/** 这里的 Mesh 用于给玩家看，也要用于碰撞检测 */
	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UStaticMeshComponent* MeshComp;

	/** 被击中时的处理函数 */
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void HandleHit(APawn* Shooter);
};
