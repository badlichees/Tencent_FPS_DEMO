#include "MyFPSPlayerController.h"
#include "FPS.h"

AMyFPSPlayerController::AMyFPSPlayerController()
{
	// 构造函数
}

void AMyFPSPlayerController::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogFPS, Log, TEXT("MyFPSPlayerController BeginPlay! Possessing: %s"), *GetNameSafe(GetPawn()));
}

