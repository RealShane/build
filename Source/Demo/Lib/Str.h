// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 工具类
 */
class DEMO_API Str
{

public:
    bool static IsSideContain(FString Value);
    bool static IsBuildContain(FString Value);
    bool static IsContain(FString Value, FString Key);
};
