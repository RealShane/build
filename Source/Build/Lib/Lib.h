// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "Build/Build/Struct/BlockActor.h"
#include "Build/Build/Struct/BuildCache.h"

/**
 * 工具类
 */
class BUILD_API Lib
{

public:
    void static echo(float Value, bool IsGoLog = true);
	void static echo(FString Value, bool IsGoLog = true);
	FBlockActor static SetSide(FString Key, FBlockActor Side, bool IsRemove = false);
	FBuildCache static SetSide(FString Key, FBuildCache Side, bool IsRemove = false, bool IsWall = false);
};
