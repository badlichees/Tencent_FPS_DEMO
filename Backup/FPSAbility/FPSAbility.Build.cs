// Source/FPSAbility/FPSAbility.Build.cs

using UnrealBuildTool;

public class FPSAbility : ModuleRules 
{
    public FPSAbility(ReadOnlyTargetRules Target) : base(Target) 
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] {
                "Core",
                "CoreUObject",
                "Engine",
                "FPS",
                "GameplayAbilities",    // <--- GAS 核心
                "GameplayTags",         // <--- 用于定义 Gameplay 状态标签
                "GameplayTasks"         // <--- 异步任务处理
        });

        // 必须添加
        PrivateDependencyModuleNames.AddRange(new string[] { "UMG" });
    }
}