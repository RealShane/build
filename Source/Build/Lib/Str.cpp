#include "Str.h"

bool FStr::IsSideContain(const FString Value)
{
	if (IsContain(Value, "Right")) {
		return true;
	} else if (IsContain(Value, "Low")) {
		return true;
	} else if (IsContain(Value, "Left")) {
		return true;
	} else if (IsContain(Value, "Up")) {
		return true;
	} else {
		return false;
	}
}

bool FStr::IsBuildContain(const FString Value)
{
	if (IsContain(Value, "Foundation")) {
		return true;
	} else if (IsContain(Value, "Wall")) {
		return true;
	} else if (IsContain(Value, "Floor")) {
		return true;
	} else {
		return false;
	}
}

bool FStr::IsOverlapContain(const FString Value)
{
	if (IsContain(Value, "BoxComponent")) {
		return true;
	} else if (IsContain(Value, "CollisionCylinder")) {
		return true;
	} else if (IsContain(Value, "")) {
		return true;
	} else {
		return false;
	}
}

bool FStr::IsContain(const FString Value, const FString Key)
{
	return Value . Contains(Key);
}
