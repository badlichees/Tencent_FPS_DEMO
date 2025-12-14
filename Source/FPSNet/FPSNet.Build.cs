// Source/FPSNet/FPSNet.Build.cs

using UnrealBuildTool;

public class FPSNet : ModuleRules 
{
    public FPSNet(ReadOnlyTargetRules Target) : base(Target) 
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] {
                "Core",
                "CoreUObject",
                "Engine",
                "NetCore",      // <--- 专门用于网络底层操作的模块
                "FPS"           // <--- 必须依赖主游戏模块，才能引用主模块中的类
        });

        PrivateDependencyModuleNames.AddRange(new string[] {
                // 如果在.cpp中使用了其他仅供内部的模块，可添加在这里
        });
    }
}