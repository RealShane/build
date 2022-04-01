// Fill out your copyright notice in the Description page of Project Settings.


#include "Str.h"

bool Str::IsSideContain(FString Value)
{
	if (IsContain(Value, "Right")) {
		return true;
	}else if (IsContain(Value, "Low")) {
		return true;
	}else if (IsContain(Value, "Left")) {
		return true;
	}else if (IsContain(Value, "Up")) {
		return true;
	}else {
		return false;
	}
}

bool Str::IsBuildContain(FString Value)
{
	if (IsContain(Value, "Foundation")) {
		return true;
	}else if (IsContain(Value, "Wall")) {
		return true;
	}else if (IsContain(Value, "Floor")) {
		return true;
	}else {
		return false;
	}
}

bool Str::IsOverlapContain(FString Value)
{
	if (IsContain(Value, "BoxComponent")) {
		return true;
	}else if (IsContain(Value, "CollisionCylinder")) {
		return true;
	}else if (IsContain(Value, "")) {
		return true;
	}else {
		return false;
	}
}

bool Str::IsContain(FString Value, FString Key)
{
	return Value.Contains(Key);
}
