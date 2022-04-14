#pragma once

#include "CoreMinimal.h"

/**
 * 工具类
 */
class BUILD_API FStr
{
public:
	bool static IsSideContain(FString Value);
	bool static IsBuildContain(FString Value);
	bool static IsOverlapContain(FString Value);
	bool static IsContain(FString Value, FString Key);
};
