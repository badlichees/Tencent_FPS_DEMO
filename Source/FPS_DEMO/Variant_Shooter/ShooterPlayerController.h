#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ShooterPlayerController.generated.h"

class UInputMappingContext;
class AShooterCharacter;
class UShooterBulletCounterUI;

/**
 *  第一人称射击游戏的基础 PlayerController（玩家控制器）
 *  管理输入映射
 *  当玩家 Pawn 被销毁时重新生成
 */
UCLASS(abstract, config="Game")
class FPS_DEMO_API AShooterPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:

	/** 此玩家的输入映射上下文 */
	UPROPERTY(EditAnywhere, Category="Input|Input Mappings")
	TArray<UInputMappingContext*> DefaultMappingContexts;

	/** 输入映射上下文 */
	UPROPERTY(EditAnywhere, Category="Input|Input Mappings")
	TArray<UInputMappingContext*> MobileExcludedMappingContexts;

	/** 要生成的移动端控制微件（Widget）类型 */
	UPROPERTY(EditAnywhere, Category="Input|Touch Controls")
	TSubclassOf<UUserWidget> MobileControlsWidgetClass;

	/** 指向移动端控制微件的指针 */
	UPROPERTY()
	TObjectPtr<UUserWidget> MobileControlsWidget;

	/** 如果为 true，即使不在移动平台上运行，玩家也将强制使用 UMG 触摸控制 */
	UPROPERTY(EditAnywhere, Config, Category = "Input|Touch Controls")
	bool bForceTouchControls = false;

	/** 当拥有的 Pawn 被销毁时要重新生成的角色类 */
	UPROPERTY(EditAnywhere, Category="Shooter|Respawn")
	TSubclassOf<AShooterCharacter> CharacterClass;

	/** 要生成的子弹计数器 UI 微件类型 */
	UPROPERTY(EditAnywhere, Category="Shooter|UI")
	TSubclassOf<UShooterBulletCounterUI> BulletCounterUIClass;

	/** 授予拥有的 Pawn 以将其标记为玩家的标签 */
	UPROPERTY(EditAnywhere, Category="Shooter|Player")
	FName PlayerPawnTag = FName("Player");

	/** 指向子弹计数器 UI 微件的指针 */
	UPROPERTY()
	TObjectPtr<UShooterBulletCounterUI> BulletCounterUI;

protected:

	/** 游戏初始化 */
	virtual void BeginPlay() override;

	/** 初始化输入绑定 */
	virtual void SetupInputComponent() override;

	/** Pawn 初始化 */
	virtual void OnPossess(APawn* InPawn) override;

	/** 当拥有的 Pawn 被销毁时调用 */
	UFUNCTION()
	void OnPawnDestroyed(AActor* DestroyedActor);

	/** 当拥有的 Pawn 的子弹计数更新时调用 */
	UFUNCTION()
	void OnBulletCountUpdated(int32 MagazineSize, int32 Bullets);

	/** 当拥有的 Pawn 受伤时调用 */
	UFUNCTION()
	void OnPawnDamaged(float LifePercent);

	/** 如果玩家应当使用 UMG 触摸控制，则返回 true */
	bool ShouldUseTouchControls() const;
};
