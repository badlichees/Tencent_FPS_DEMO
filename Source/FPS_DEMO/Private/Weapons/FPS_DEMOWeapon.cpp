#include "Weapons/FPS_DEMOWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Interfaces/FPS_DEMOWeaponInterface.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/DamageEvents.h"
#include "TimerManager.h"

AFPS_DEMOWeapon::AFPS_DEMOWeapon()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));

	WeaponMeshFP = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMeshFP"));
	WeaponMeshFP->SetupAttachment(RootComponent);
	WeaponMeshFP->SetOnlyOwnerSee(true);

	WeaponMeshTP = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMeshTP"));
	WeaponMeshTP->SetupAttachment(RootComponent);
	WeaponMeshTP->SetOwnerNoSee(true);
}

void AFPS_DEMOWeapon::BeginPlay()
{
	Super::BeginPlay();
	WeaponOwner = Cast<IFPS_DEMOWeaponInterface>(GetOwner());
}

void AFPS_DEMOWeapon::StartFiring()
{
	if (bIsFiring) return;
	bIsFiring = true;

	// 立即执行第一发
	FireOnce();

	// 如果是全自动武器，开启循环射击计时器
	if (bIsAutomatic && FireRate > 0)
	{
		const float FireInterval = 60.0f / FireRate;
		GetWorldTimerManager().SetTimer(FireTimerHandle, this, &AFPS_DEMOWeapon::FireOnce, FireInterval, true);
	}
}

void AFPS_DEMOWeapon::StopFiring()
{
	bIsFiring = false;
	GetWorldTimerManager().ClearTimer(FireTimerHandle);
}

void AFPS_DEMOWeapon::FireOnce()
{
	if (!WeaponOwner) return;

	// 1. 获取枪口位置
	const FVector MuzzleLocation = WeaponMeshFP->GetSocketLocation(MuzzleSocketName);
	
	// 2. 获取目标位置（从角色相机射出的射线命中点）
	const FVector TargetLocation = WeaponOwner->GetWeaponTargetLocation();
	
	// 3. 计算射击方向
	const FVector ShotDirection = (TargetLocation - MuzzleLocation).GetSafeNormal();
	const FVector TraceEnd = MuzzleLocation + (ShotDirection * MaxRange);

	// 4. 执行射线检测
	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	QueryParams.AddIgnoredActor(GetOwner());

	const bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		MuzzleLocation,
		TraceEnd,
		ECC_Visibility,
		QueryParams
	);

	// 5. 生成枪口特效
	if (MuzzleFlashEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAttached(
			MuzzleFlashEffect,
			WeaponMeshFP,
			MuzzleSocketName,
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			EAttachLocation::SnapToTarget,
			true
		);
	}

	// 6. 播放开火音效
	if (FireSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, MuzzleLocation);
	}

	// 7. 如果命中目标
	if (bHit)
	{
		// 生成击中特效
		if (ImpactEffect)
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(
				GetWorld(),
				ImpactEffect,
				HitResult.ImpactPoint,
				HitResult.ImpactNormal.Rotation()
			);
		}

		// 对命中的 Actor 施加伤害
		if (AActor* HitActor = HitResult.GetActor())
		{
			FPointDamageEvent DamageEvent(Damage, HitResult, ShotDirection, nullptr);
			AController* InstigatorController = GetInstigatorController();
			HitActor->TakeDamage(Damage, DamageEvent, InstigatorController, this);
		}
	}

	// 8. 通知角色播放开火动画和后坐力（可选）
	// WeaponOwner->PlayFiringMontage(FireMontage);
	// WeaponOwner->AddWeaponRecoil(RecoilAmount);
}

void AFPS_DEMOWeapon::ActivateWeapon()
{
	SetActorHiddenInGame(false);

	if (WeaponOwner)
	{
		WeaponOwner->AttachWeaponMeshes(this);
		WeaponOwner->OnWeaponActivated(this);
	}
}

void AFPS_DEMOWeapon::DeactivateWeapon()
{
	SetActorHiddenInGame(true);
	StopFiring();
}

