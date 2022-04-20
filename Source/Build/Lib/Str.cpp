#include "Str.h"

bool FStr::IsSideContain(const FString Value)
{
	if (IsContain(Value, FStatic::Right)) {
		return true;
	}
	if (IsContain(Value, FStatic::Low)) {
		return true;
	}
	if (IsContain(Value, FStatic::Left)) {
		return true;
	}
	if (IsContain(Value, FStatic::Up)) {
		return true;
	}
	return false;
}

bool FStr::IsBuildContain(const FString Value)
{
	if (IsContain(Value, FStatic::Foundation)) {
		return true;
	}
	if (IsContain(Value, FStatic::Wall)) {
		return true;
	}
	if (IsContain(Value, FStatic::Floor)) {
		return true;
	}
	return false;
}

bool FStr::IsOverlapContain(const FString Value)
{
	if (IsContain(Value, FStatic::BoxComponent)) {
		return true;
	}
	if (IsContain(Value, FStatic::CollisionCylinder)) {
		return true;
	}
	return false;
}

bool FStr::IsContain(const FString Value, const FString Key)
{
	return Value . Contains(Key);
}
