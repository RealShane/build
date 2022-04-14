#include "Lib.h"

void FLib::Echo(const float Value, const bool IsGoLog)
{
	const FString Message = FString::Printf(TEXT("%f"), Value);
	if (IsGoLog) {
		UE_LOG(LogTemp, Warning, TEXT("%s"), *Message);
	} else {
		GEngine -> AddOnScreenDebugMessage(-1, 1.f, FColor::Green, Message);
	}
}

void FLib::Echo(const FString Value, const bool IsGoLog)
{
	const FString Message = FString::Printf(TEXT("%s"), *Value);
	if (IsGoLog) {
		UE_LOG(LogTemp, Warning, TEXT("%s"), *Message);
	} else {
		GEngine -> AddOnScreenDebugMessage(-1, 1.f, FColor::Green, Message);
	}
}

FBlockActor FLib::SetSide(const FString Key, FBlockActor Side, const bool IsRemove)
{
	if (!IsRemove) {
		if (Key == "Right") {
			Side . Right = true;
		}
		if (Key == "Low") {
			Side . Low = true;
		}
		if (Key == "Left") {
			Side . Left = true;
		}
		if (Key == "Up") {
			Side . Up = true;
		}
	} else {
		if (Key == "Right") {
			Side . Right = false;
		}
		if (Key == "Low") {
			Side . Low = false;
		}
		if (Key == "Left") {
			Side . Left = false;
		}
		if (Key == "Up") {
			Side . Up = false;
		}
	}
	return Side;
}

FBuildCache FLib::SetSide(const FString Key, FBuildCache Side, const bool IsRemove, const bool IsWall)
{
	if (!IsRemove) {
		if (IsWall) {
			if (Key == "Right") {
				Side . WallRight = true;
			}
			if (Key == "Low") {
				Side . WallLow = true;
			}
			if (Key == "Left") {
				Side . WallLeft = true;
			}
			if (Key == "Up") {
				Side . WallUp = true;
			}
		} else {
			if (Key == "Right") {
				Side . Right = true;
			}
			if (Key == "Low") {
				Side . Low = true;
			}
			if (Key == "Left") {
				Side . Left = true;
			}
			if (Key == "Up") {
				Side . Up = true;
			}
		}
	} else {
		if (IsWall) {
			if (Key == "Right") {
				Side . WallRight = false;
			}
			if (Key == "Low") {
				Side . WallLow = false;
			}
			if (Key == "Left") {
				Side . WallLeft = false;
			}
			if (Key == "Up") {
				Side . WallUp = false;
			}
		} else {
			if (Key == "Right") {
				Side . Right = false;
			}
			if (Key == "Low") {
				Side . Low = false;
			}
			if (Key == "Left") {
				Side . Left = false;
			}
			if (Key == "Up") {
				Side . Up = false;
			}
		}
	}
	return Side;
}
