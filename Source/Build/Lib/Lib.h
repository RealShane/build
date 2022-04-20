#pragma once

#include "CoreMinimal.h"
#include "Static.h"
#include "Build/Build/Struct/BlockActor.h"
#include "Build/Build/Struct/BuildCache.h"

/**
 * 工具类
 */
class BUILD_API FLib
{
public:
	void static Echo(float Value, bool IsGoLog = false);
	void static Echo(FString Value, bool IsGoLog = false);
	FBlockActor static SetSide(FString Key, FBlockActor Side, bool IsRemove = false);
	FBuildCache static SetSide(FString Key, FBuildCache Side, bool IsRemove = false, bool IsWall = false);

	static float Max(float Num1, float Num2);
};
