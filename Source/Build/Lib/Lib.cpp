#include "Lib.h"

void FLib::Echo(const float Value, const bool IsGoLog)
{
	const FString Message = FString::Printf(TEXT("%f"), Value);
	if (IsGoLog) {
		UE_LOG(LogTemp, Warning, TEXT("%s"), *Message);
	} else {
		GEngine -> AddOnScreenDebugMessage(-FStatic::One, FStatic::One, FColor::Green, Message);
	}
}

void FLib::Echo(const FString Value, const bool IsGoLog)
{
	const FString Message = FString::Printf(TEXT("%s"), *Value);
	if (IsGoLog) {
		UE_LOG(LogTemp, Warning, TEXT("%s"), *Message);
	} else {
		GEngine -> AddOnScreenDebugMessage(-FStatic::One, FStatic::One, FColor::Green, Message);
	}
}

FBlockActor FLib::SetSide(const FString Key, FBlockActor Side, const bool IsRemove)
{
	if (!IsRemove) {
		if (Key == FStatic::Right) {
			Side . Right = true;
		}
		if (Key == FStatic::Low) {
			Side . Low = true;
		}
		if (Key == FStatic::Left) {
			Side . Left = true;
		}
		if (Key == FStatic::Up) {
			Side . Up = true;
		}
		if (Key == FStatic::Front) {
			Side . Front = true;
		}
		if (Key == FStatic::Back) {
			Side . Back = true;
		}
	} else {
		if (Key == FStatic::Right) {
			Side . Right = false;
		}
		if (Key == FStatic::Low) {
			Side . Low = false;
		}
		if (Key == FStatic::Left) {
			Side . Left = false;
		}
		if (Key == FStatic::Up) {
			Side . Up = false;
		}
		if (Key == FStatic::Front) {
			Side . Front = false;
		}
		if (Key == FStatic::Back) {
			Side . Back = false;
		}
	}
	return Side;
}

FBuildCache FLib::SetSide(const FString Key, FBuildCache Side, const bool IsRemove, const bool IsWall)
{
	if (!IsRemove) {
		if (IsWall) {
			if (Key == FStatic::Right) {
				Side . WallRight = true;
			}
			if (Key == FStatic::Low) {
				Side . WallLow = true;
			}
			if (Key == FStatic::Left) {
				Side . WallLeft = true;
			}
			if (Key == FStatic::Up) {
				Side . WallUp = true;
			}
		} else {
			if (Key == FStatic::Right) {
				Side . Right = true;
			}
			if (Key == FStatic::Low) {
				Side . Low = true;
			}
			if (Key == FStatic::Left) {
				Side . Left = true;
			}
			if (Key == FStatic::Up) {
				Side . Up = true;
			}
			if (Key == FStatic::Front) {
				Side . Front = true;
			}
			if (Key == FStatic::Back) {
				Side . Back = true;
			}
		}
	} else {
		if (IsWall) {
			if (Key == FStatic::Right) {
				Side . WallRight = false;
			}
			if (Key == FStatic::Low) {
				Side . WallLow = false;
			}
			if (Key == FStatic::Left) {
				Side . WallLeft = false;
			}
			if (Key == FStatic::Up) {
				Side . WallUp = false;
			}
		} else {
			if (Key == FStatic::Right) {
				Side . Right = false;
			}
			if (Key == FStatic::Low) {
				Side . Low = false;
			}
			if (Key == FStatic::Left) {
				Side . Left = false;
			}
			if (Key == FStatic::Up) {
				Side . Up = false;
			}
			if (Key == FStatic::Front) {
				Side . Front = false;
			}
			if (Key == FStatic::Back) {
				Side . Back = false;
			}
		}
	}
	return Side;
}

float FLib::Max(const float Num1, const float Num2)
{
	return Num1 > Num2 ? Num1 : Num2;
}
