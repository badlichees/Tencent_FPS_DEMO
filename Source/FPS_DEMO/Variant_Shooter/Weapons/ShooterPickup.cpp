#include "ShooterPickup.h"
#include "Components/SceneComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "ShooterWeaponHolder.h"
#include "ShooterWeapon.h"
#include "Engine/World.h"
#include "TimerManager.h"

// 创建拾取物的“身体”
AShooterPickup::AShooterPickup()
{
 	PrimaryActorTick.bCanEverTick = true;

	// 创建根组件
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	// 创建碰撞球体
	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collision"));
	SphereCollision->SetupAttachment(RootComponent);

	SphereCollision->SetRelativeLocation(FVector(0.0f, 0.0f, 84.0f));
	SphereCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereCollision->SetCollisionObjectType(ECC_WorldStatic);
	SphereCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	SphereCollision->bFillCollisionUnderneathForNavmesh = true;

	// 订阅球体碰撞重叠事件
	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &AShooterPickup::OnOverlap);

	// 创建网格体
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(SphereCollision);

	Mesh->SetCollisionProfileName(FName("NoCollision"));
}

// UE中的生命周期函数，在编辑器中构建或更新Actor时调用
void AShooterPickup::OnConstruction(const FTransform& Transform)
{
	// 调用父类AActor的构造函数，确保先进行基础的构建逻辑
	Super::OnConstruction(Transform);

	// 通过声明基于头文件中自定义的FWeaponTableRow的WeaponType结构体，获取武器数据，获取成功则设置网格体
	if (FWeaponTableRow* WeaponData = WeaponType.GetRow<FWeaponTableRow>(FString()))
	{
		Mesh->SetStaticMesh(WeaponData->StaticMesh.LoadSynchronous());
	}
}

// 开始游戏时，会在获取到武器数据后进行类的复制
void AShooterPickup::BeginPlay()
{
	Super::BeginPlay();

	if (FWeaponTableRow* WeaponData = WeaponType.GetRow<FWeaponTableRow>(FString()))
	{
		WeaponClass = WeaponData->WeaponToSpawn;
	}
}

void AShooterPickup::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	// 清除重生计时器
	GetWorld()->GetTimerManager().ClearTimer(RespawnTimer);
}

// 当与球体发生碰撞时，会调用此函数
void AShooterPickup::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 我们是否与武器持有者发生了碰撞？
	if (IShooterWeaponHolder* WeaponHolder = Cast<IShooterWeaponHolder>(OtherActor))
	{
		WeaponHolder->AddWeaponClass(WeaponClass);

		// 隐藏此网格体
		SetActorHiddenInGame(true);

		// 禁用碰撞
		SetActorEnableCollision(false);

		// 禁用 Tick
		SetActorTickEnabled(false);

		// 安排重生
		GetWorld()->GetTimerManager().SetTimer(RespawnTimer, this, &AShooterPickup::RespawnPickup, RespawnTime, false);
	}
}

// 拾取物重生时会显示隐藏的网格体并调用蓝图
void AShooterPickup::RespawnPickup()
{
	SetActorHiddenInGame(false);
	BP_OnRespawn();
}

void AShooterPickup::FinishRespawn()
{
	// 启用碰撞
	SetActorEnableCollision(true);

	// 启用 Tick
	SetActorTickEnabled(true);
}
