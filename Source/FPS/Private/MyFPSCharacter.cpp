#include "MyFPSCharacter.h"
#include "EnhancedInputComponent.h"
#include "DrawDebugHelpers.h"
#include "FPS.h"
#include "MyWeapon.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/CharacterMovementComponent.h"

AMyFPSCharacter::AMyFPSCharacter()
{
	// 启用 Tick 以更新动画数据
	PrimaryActorTick.bCanEverTick = true;
}

void AMyFPSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 1. 更新速度 (只要水平速度)
	FVector Velocity = GetVelocity();
	Velocity.Z = 0;
	MovementSpeed = Velocity.Size();

	// 2. 更新跳跃状态
	if (GetCharacterMovement())
	{
		bIsFalling = GetCharacterMovement()->IsFalling();
	}
}

void AMyFPSCharacter::BeginPlay()
{
	Super::BeginPlay();

	// 只在服务器生成武器并同步给客户端
	if (GetLocalRole() == ROLE_Authority && DefaultWeaponClass)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();

		CurrentWeapon = GetWorld()->SpawnActor<AMyWeapon>(DefaultWeaponClass, SpawnParams);
		if (CurrentWeapon)
		{
			// 将武器附加到角色的第一人称手臂上
			// 使用现有的 HandGrip_R 插槽
			CurrentWeapon->AttachToComponent(GetFirstPersonMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("HandGrip_R"));
		
			// [姿态优化] 链接武器的动画叠加层
			if (CurrentWeapon->WeaponAnimLayer)
			{
				GetFirstPersonMesh()->LinkAnimClassLayers(CurrentWeapon->WeaponAnimLayer);
			}
		}
	}
}

void AMyFPSCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// 同步当前武器指针
	DOREPLIFETIME(AMyFPSCharacter, CurrentWeapon);
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
	// 通知服务器开火
	ServerDoFire();
}

void AMyFPSCharacter::ServerDoFire_Implementation()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->Fire(this);
	}
}
