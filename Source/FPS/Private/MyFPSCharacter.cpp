#include "MyFPSCharacter.h"
#include "EnhancedInputComponent.h"
#include "DrawDebugHelpers.h"
#include "FPS.h"
#include "FpTarget.h"

AMyFPSCharacter::AMyFPSCharacter()
{
	// 构造函数
}

void AMyFPSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// 绑定开火按键 (Triggered or Started)
		if (FireAction)
		{
			EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &AMyFPSCharacter::DoFire);
		}
	}
}

void AMyFPSCharacter::DoAim(float Yaw, float Pitch)
{
	if (GetController())
	{
		// 在这里统一乘以灵敏度系数
		AddControllerYawInput(Yaw * MouseSensitivity);
		AddControllerPitchInput(Pitch * MouseSensitivity);
	}
}

void AMyFPSCharacter::DoFire()
{
	// 1. 获取相机位置和方向
	FVector CameraLocation;
	FRotator CameraRotation;
	GetActorEyesViewPoint(CameraLocation, CameraRotation);

	// 2. 计算射线终点
	FVector EndLocation = CameraLocation + (CameraRotation.Vector() * WeaponRange);

	// 3. 射线检测参数
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this); // 忽略自己

	// 4. 执行射线检测
	FHitResult Hit;
	bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, CameraLocation, EndLocation, ECC_Visibility, QueryParams);

	// 5. 处理结果
	if (bHit)
	{
		// 画一个红点在击中位置 (调试用)
		DrawDebugPoint(GetWorld(), Hit.Location, 10.0f, FColor::Red, false, 2.0f);
		
		AActor* HitActor = Hit.GetActor();
		if (HitActor)
		{
			UE_LOG(LogFPS, Log, TEXT("Hit Actor: %s"), *HitActor->GetName());
			
			// 尝试将击中物体转换为靶子类
			if (AFpTarget* Target = Cast<AFpTarget>(HitActor))
			{
				// 如果是靶子，就让它处理“被击中”的逻辑 (加分、自毁)
				Target->HandleHit(this);
			}
		}
	}
	else
	{
		// 没打中，画一条线表示弹道
		DrawDebugLine(GetWorld(), CameraLocation, EndLocation, FColor::Yellow, false, 2.0f);
	}
}
