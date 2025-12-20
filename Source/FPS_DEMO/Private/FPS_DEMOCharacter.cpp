#include "FPS_DEMOCharacter.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "FPS_DEMO.h"

// 构造函数
AFPS_DEMOCharacter::AFPS_DEMOCharacter()
{
	// 设置碰撞胶囊体的大小
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
	
	// 创建仅此角色所有者可见的第一人称网格体（手臂）
	FirstPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("First Person Mesh"));

	// 设置手臂的父组件为全身模型
	FirstPersonMesh->SetupAttachment(GetMesh());
	FirstPersonMesh->SetOnlyOwnerSee(true);
	FirstPersonMesh->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::FirstPerson;
	FirstPersonMesh->SetCollisionProfileName(FName("NoCollision"));

	// 创建相机组件
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("First Person Camera"));
	FirstPersonCameraComponent->SetupAttachment(FirstPersonMesh, FName("head"));
	FirstPersonCameraComponent->SetRelativeLocationAndRotation(FVector(-2.8f, 5.89f, 0.0f), FRotator(0.0f, 90.0f, -90.0f));
	FirstPersonCameraComponent->bUsePawnControlRotation = true;
	FirstPersonCameraComponent->bEnableFirstPersonFieldOfView = true;
	FirstPersonCameraComponent->bEnableFirstPersonScale = true;
	FirstPersonCameraComponent->FirstPersonFieldOfView = 70.0f;
	FirstPersonCameraComponent->FirstPersonScale = 0.6f;

	// 配置角色组件
	GetMesh()->SetOwnerNoSee(true);
	GetMesh()->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::WorldSpaceRepresentation;

	GetCapsuleComponent()->SetCapsuleSize(34.0f, 96.0f); // 配置胶囊体大小

	// 配置角色移动
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;
	GetCharacterMovement()->AirControl = 0.5f;
}

// 设置玩家输入组件
void AFPS_DEMOCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{	
	// 设置输入动作绑定
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// 跳跃
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AFPS_DEMOCharacter::DoJumpStart);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AFPS_DEMOCharacter::DoJumpEnd);

		// 移动
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AFPS_DEMOCharacter::MoveInput);

		// 视角查看/瞄准
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AFPS_DEMOCharacter::LookInput);
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &AFPS_DEMOCharacter::LookInput);
	}
	else
	{
		// 如果找不到Enhanced Input Component，输出错误日志
		UE_LOG(LogMyDEMO, Error, TEXT("“%s”找不到增强输入组件 (Enhanced Input Component)！此模板是基于增强输入系统构建的。如果您打算使用旧版输入系统，则需要更新此 C++ 文件。"), *GetNameSafe(this));
	}
}

// 移动输入
void AFPS_DEMOCharacter::MoveInput(const FInputActionValue& Value)
{
	// 获取 Vector2D 移动轴
	FVector2D MovementVector = Value.Get<FVector2D>();

	// 将轴值传递给移动输入
	DoMove(MovementVector.X, MovementVector.Y);

}

// 视角移动输入
void AFPS_DEMOCharacter::LookInput(const FInputActionValue& Value)
{
	// 获取 Vector2D 查看轴
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// 将轴值传递给瞄准输入
	DoAim(LookAxisVector.X, LookAxisVector.Y);

}

// 瞄准输入
void AFPS_DEMOCharacter::DoAim(float Yaw, float Pitch)
{
	if (GetController())
	{
		// 传递旋转输入
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}

// 移动输入
void AFPS_DEMOCharacter::DoMove(float Right, float Forward)
{
	if (GetController())
	{
		// 传递移动输入
		AddMovementInput(GetActorRightVector(), Right);
		AddMovementInput(GetActorForwardVector(), Forward);
	}
}

// 起跳输入
void AFPS_DEMOCharacter::DoJumpStart()
{
	// 向角色传递跳跃指令
	Jump();
}

// 起跳结束输入
void AFPS_DEMOCharacter::DoJumpEnd()
{
	// 向角色传递停止跳跃指令
	StopJumping();
}
