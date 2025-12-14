// Source/FPSAI/FPSAI.Build.cs

using UnrealBuildTool;

public class FPSAI : ModuleRules 
{
    public FPSAI(ReadOnlyTargetRules Target) : base(Target) 
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] {
                "Core",
                "CoreUObject",
                "Engine",
                "FPS",
                "AIModule",             // <--- AI 核心，如 AIController, AIPerception
                "GameplayTasks",        // <--- 行为树和服务节点的基础
                "NavigationSystem",     // <--- 路径寻路和导航系统
                "DeveloperSettings"     // <--- 如果需要自定义 AI 配置（可选）
        });

        PrivateDependencyModuleNames.AddRange(new string[] { });
    }
}