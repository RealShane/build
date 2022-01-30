// Fill out your copyright notice in the Description page of Project Settings.


#include "Str.h"

bool Str::IsBuildContain(FString Value)
{
	if (IsContain(Value, "Floor")) {
		return true;
	}else if (IsContain(Value, "Floor")) {
		return true;
	}else {
		return false;
	}
}

bool Str::IsContain(FString Value, FString Key)
{
	return Value.Contains(Key);
}
