#include "Pickups/FPS_DEMOPickup.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Interfaces/FPS_DEMOWeaponInterface.h"
#include "TimerManager.h"

AFPS_DEMOPickup::AFPS_DEMOPickup()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	SphereCollision->SetupAttachment(RootComponent);
	SphereCollision->SetCollisionProfileName(FName("Trigger"));
	SphereCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	
	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &AFPS_DEMOPickup::OnOverlap);

	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickupMesh"));
	PickupMesh->SetupAttachment(SphereCollision);
	PickupMesh->SetCollisionProfileName(FName("NoCollision"));
}

void AFPS_DEMOPickup::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (FFPS_DEMOWeaponTableRow* WeaponData = WeaponType.GetRow<FFPS_DEMOWeaponTableRow>(FString()))
	{
		PickupMesh->SetStaticMesh(WeaponData->StaticMesh.LoadSynchronous());
	}
}

void AFPS_DEMOPickup::BeginPlay()
{
	Super::BeginPlay();

	if (FFPS_DEMOWeaponTableRow* WeaponData = WeaponType.GetRow<FFPS_DEMOWeaponTableRow>(FString()))
	{
		WeaponClass = WeaponData->WeaponToSpawn;
	}
}

void AFPS_DEMOPickup::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 关键：通过接口识别“武器持有者”
	if (IFPS_DEMOWeaponInterface* WeaponHolder = Cast<IFPS_DEMOWeaponInterface>(OtherActor))
	{
		WeaponHolder->AddWeaponClass(WeaponClass);

		// 拾取后的反馈逻辑
		SetActorHiddenInGame(true);
		SetActorEnableCollision(false);

		// 开启重生计时器
		GetWorldTimerManager().SetTimer(RespawnTimer, this, &AFPS_DEMOPickup::RespawnPickup, RespawnTime, false);
	}
}

void AFPS_DEMOPickup::RespawnPickup()
{
	SetActorHiddenInGame(false);
	BP_OnRespawn();
}

void AFPS_DEMOPickup::FinishRespawn()
{
	SetActorEnableCollision(true);
}
