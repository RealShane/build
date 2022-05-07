#pragma once

#include "CoreMinimal.h"
#include "Static.h"
#include "Build/Build/Struct/BlockActor.h"
#include "Build/Build/Struct/Buildings.h"

/**
 * 工具类
 */
class BUILD_API FStr
{
public:
	bool static IsSideContain(FString Value);
	bool static IsBuildContain(FString Value);
	bool static IsOverlapContain(FString Value);
	bool static IsSideBlock(FString Side, const FBuildings* BuildBlock);
	bool static IsWallSideBlock(FString Side, const FBuildings* BuildBlock);
	bool static IsLand(FString Value);
	bool static IsContain(FString Value, FString Key);

	FBlockActor static SetSide(FString Key, FBlockActor Side, bool IsRemove = false);
	FBuildings static SetSide(FString Key, FBuildings Side, bool IsRemove = false, bool IsWall = false);
};
