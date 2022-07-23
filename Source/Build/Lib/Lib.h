#pragma once

#include "CoreMinimal.h"
#include "Static.h"

/**
 * 工具类
 */
class BUILD_API FLib
{
public:
	void static Echo(float Value, bool IsGoLog = true, int Time = 1);
	void static Echo(FString Value, bool IsGoLog = true, int Time = 1);
	float static Max(float Num1, float Num2);
	FString static Salt(int Bit);
};
