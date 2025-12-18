#include "MyWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "FpTarget.h"
#include "FPS.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

AMyWeapon::AMyWeapon()
{
	// 创建武器网格体组件
	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	RootComponent = MeshComp;

	// 开启网络复制
	SetReplicates(true);
}

void AMyWeapon::BeginPlay()
{
	Super::BeginPlay();
}

void AMyWeapon::Fire(APawn* Shooter)
{
	if (!Shooter) return;

	// 1. 播放本地特效 (如果是 ListenServer 玩家) 或通知所有客户端播放
	Multi_PlayFireEffects();

	// 2. 只在服务器执行命中判定
	if (GetLocalRole() < ROLE_Authority) return;

	// 获取相机位置和方向
	FVector CameraLocation;
	FRotator CameraRotation;
	Shooter->GetActorEyesViewPoint(CameraLocation, CameraRotation);

	FVector EndLocation = CameraLocation + (CameraRotation.Vector() * WeaponRange);

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	QueryParams.AddIgnoredActor(Shooter);

	FHitResult Hit;
	bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, CameraLocation, EndLocation, ECC_Visibility, QueryParams);

	if (bHit)
	{
		// 恢复红点视觉反馈 (调试用)
		DrawDebugPoint(GetWorld(), Hit.Location, 10.0f, FColor::Red, false, 2.0f);

		AActor* HitActor = Hit.GetActor();
		if (HitActor)
		{
			UE_LOG(LogFPS, Log, TEXT("Weapon Hit: %s"), *HitActor->GetName());

			if (AFpTarget* Target = Cast<AFpTarget>(HitActor))
			{
				Target->HandleHit(Shooter);
			}
		}
	}
}

void AMyWeapon::Multi_PlayFireEffects_Implementation()
{
	// 播放枪口火光 (Niagara)
	if (MuzzleFlash)
	{
		UNiagaraFunctionLibrary::SpawnSystemAttached(MuzzleFlash, MeshComp, TEXT("Muzzle"), FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget, true);
	}

	// 播放音效
	if (FireSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}
}
