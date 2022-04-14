#pragma once

#include "CoreMinimal.h"
#include "Build/Build/Struct/BlockActor.h"
#include "Build/Build/Struct/BuildCache.h"

/**
 * 工具类
 */
class BUILD_API FLib
{
public:
	void static Echo(float Value, bool IsGoLog = true);
	void static Echo(FString Value, bool IsGoLog = true);
	FBlockActor static SetSide(FString Key, FBlockActor Side, bool IsRemove = false);
	FBuildCache static SetSide(FString Key, FBuildCache Side, bool IsRemove = false, bool IsWall = false);
};
