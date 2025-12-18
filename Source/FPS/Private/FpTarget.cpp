#include "FpTarget.h"
#include "FPS.h"
#include "GameFramework/PlayerState.h"
#include "MyFPSPlayerState.h"

AFpTarget::AFpTarget()
{
	// 1. 创建 Mesh 组件
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;

	// 2. 启用物理和碰撞，保证射线能打到
	MeshComp->SetCollisionProfileName(TEXT("BlockAll"));
}

void AFpTarget::HandleHit(APawn* Shooter)
{
	UE_LOG(LogFPS, Log, TEXT("Target Hit by: %s"), *GetNameSafe(Shooter));

	// 1. 获取射手的 PlayerState
	if (Shooter && Shooter->GetPlayerState())
	{
		AMyFPSPlayerState* PS = Cast<AMyFPSPlayerState>(Shooter->GetPlayerState());
		if (PS)
		{
			// 2. 加分!
			PS->AddScore(10.0f);
		}
	}

	// 3. 销毁靶子 (表示被打掉了)
	Destroy();
}
