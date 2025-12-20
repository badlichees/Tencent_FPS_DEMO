#include "Variant_Shooter/AI/ShooterNPC.h"
#include "ShooterWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/World.h"
#include "ShooterGameMode.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TimerManager.h"

void AShooterNPC::BeginPlay()
{
	Super::BeginPlay();

	// 生成武器
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	Weapon = GetWorld()->SpawnActor<AShooterWeapon>(WeaponClass, GetActorTransform(), SpawnParams);
}

void AShooterNPC::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	// 清除死亡计时器
	GetWorld()->GetTimerManager().ClearTimer(DeathTimer);
}

float AShooterNPC::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	// 如果已经死亡则忽略
	if (bIsDead)
	{
		return 0.0f;
	}

	// 减少 HP
	CurrentHP -= Damage;

	// HP 是否耗尽？
	if (CurrentHP <= 0.0f)
	{
		Die();
	}

	return Damage;
}

void AShooterNPC::AttachWeaponMeshes(AShooterWeapon* WeaponToAttach)
{
	const FAttachmentTransformRules AttachmentRule(EAttachmentRule::SnapToTarget, false);

	// 附加武器 Actor
	WeaponToAttach->AttachToActor(this, AttachmentRule);

	// 附加武器网格体
	WeaponToAttach->GetFirstPersonMesh()->AttachToComponent(GetFirstPersonMesh(), AttachmentRule, FirstPersonWeaponSocket);
	WeaponToAttach->GetThirdPersonMesh()->AttachToComponent(GetMesh(), AttachmentRule, ThirdPersonWeaponSocket);
}

void AShooterNPC::PlayFiringMontage(UAnimMontage* Montage)
{
	// 未使用
}

void AShooterNPC::AddWeaponRecoil(float Recoil)
{
	// 未使用
}

void AShooterNPC::UpdateWeaponHUD(int32 CurrentAmmo, int32 MagazineSize)
{
	// 未使用
}

FVector AShooterNPC::GetWeaponTargetLocation()
{
	// 从摄像机位置开始瞄准
	const FVector AimSource = GetFirstPersonCameraComponent()->GetComponentLocation();

	FVector AimDir, AimTarget = FVector::ZeroVector;

	// 我们是否有瞄准目标？
	if (CurrentAimTarget)
	{
		// 瞄准 Actor 的位置
		AimTarget = CurrentAimTarget->GetActorLocation();

		// 应用垂直偏移以瞄准头部/脚部
		AimTarget.Z += FMath::RandRange(MinAimOffsetZ, MaxAimOffsetZ);

		// 获取瞄准方向并应用锥形随机偏移
		AimDir = (AimTarget - AimSource).GetSafeNormal();
		AimDir = UKismetMathLibrary::RandomUnitVectorInConeInDegrees(AimDir, AimVarianceHalfAngle);

		
	} else {

		// 没有瞄准目标，则直接使用摄像机朝向
		AimDir = UKismetMathLibrary::RandomUnitVectorInConeInDegrees(GetFirstPersonCameraComponent()->GetForwardVector(), AimVarianceHalfAngle);

	}

	// 计算无阻碍的瞄准目标位置
	AimTarget = AimSource + (AimDir * AimRange);

	// 运行可见性追踪以检查是否有阻碍
	FHitResult OutHit;

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	GetWorld()->LineTraceSingleByChannel(OutHit, AimSource, AimTarget, ECC_Visibility, QueryParams);

	// 返回命中点或追踪终点
	return OutHit.bBlockingHit ? OutHit.ImpactPoint : OutHit.TraceEnd;
}

void AShooterNPC::AddWeaponClass(const TSubclassOf<AShooterWeapon>& InWeaponClass)
{
	// 未使用
}

void AShooterNPC::OnWeaponActivated(AShooterWeapon* InWeapon)
{
	// 未使用
}

void AShooterNPC::OnWeaponDeactivated(AShooterWeapon* InWeapon)
{
	// 未使用
}

void AShooterNPC::OnSemiWeaponRefire()
{
	// 我们还在射击吗？
	if (bIsShooting)
	{
		// 发射武器
		Weapon->StartFiring();
	}
}

void AShooterNPC::Die()
{
	// 如果已经死亡则忽略
	if (bIsDead)
	{
		return;
	}

	// 升起死亡标志
	bIsDead = true;

	// 为该角色授予死亡标签
	Tags.Add(DeathTag);

	// 调用委托
	OnPawnDeath.Broadcast();

	// 增加阵营得分值
	if (AShooterGameMode* GM = Cast<AShooterGameMode>(GetWorld()->GetAuthGameMode()))
	{
		GM->IncrementTeamScore(TeamByte);
	}

	// 禁用胶囊体碰撞
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// 停止移动
	GetCharacterMovement()->StopMovementImmediately();
	GetCharacterMovement()->StopActiveMovement();

	// 在第三人称网格体上开启布娃娃物理
	GetMesh()->SetCollisionProfileName(RagdollCollisionProfile);
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetPhysicsBlendWeight(1.0f);

	// 安排 Actor 销毁
	GetWorld()->GetTimerManager().SetTimer(DeathTimer, this, &AShooterNPC::DeferredDestruction, DeferredDestructionTime, false);
}

void AShooterNPC::DeferredDestruction()
{
	Destroy();
}

void AShooterNPC::StartShooting(AActor* ActorToShoot)
{
	// 保存瞄准目标
	CurrentAimTarget = ActorToShoot;

	// 升起标志
	bIsShooting = true;

	// 通知武器
	Weapon->StartFiring();
}

void AShooterNPC::StopShooting()
{
	// 降低标志
	bIsShooting = false;

	// 通知武器
	Weapon->StopFiring();
}
