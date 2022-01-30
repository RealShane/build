// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 工具类
 */
class DEMO_API Lib
{

public:
    void static echo(float Value, bool IsGoLog = true);
	void static echo(FString Value, bool IsGoLog = true);
};
