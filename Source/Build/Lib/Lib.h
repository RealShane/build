#pragma once

#include "CoreMinimal.h"
#include "Static.h"

/**
 * 工具类
 */
class BUILD_API FLib
{
public:
	void static Echo(float Value, bool IsGoLog = false);
	void static Echo(FString Value, bool IsGoLog = false);
	static float Max(float Num1, float Num2);
};
