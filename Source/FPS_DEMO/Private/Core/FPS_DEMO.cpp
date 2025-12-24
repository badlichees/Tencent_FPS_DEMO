#include "FPS_DEMO.h"
#include "Modules/ModuleManager.h"

// 将代码注册到Unreal引擎中（不能随便改）
IMPLEMENT_PRIMARY_GAME_MODULE( FDefaultGameModuleImpl, FPS_DEMO, "FPS_DEMO" );

// 定义日志类别（专门用于项目的日志）
DEFINE_LOG_CATEGORY(LogMyDEMO);