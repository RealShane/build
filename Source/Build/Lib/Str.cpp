#include "Str.h"

#include "Lib.h"

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
	if (IsContain(Value, FStatic::Front)) {
		return true;
	}
	if (IsContain(Value, FStatic::Back)) {
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
	if (IsContain(Value, FStatic::StaticMeshComponent)) {
		return true;
	}
	if (IsContain(Value, FStatic::BoxComponent)) {
		return true;
	}
	if (IsContain(Value, FStatic::CollisionCylinder)) {
		return true;
	}
	return false;
}

bool FStr::IsSideBlock(const FString Side, const FBuildings* BuildBlock)
{
	if (Side == FStatic::Front && BuildBlock -> Front) {
		return true;
	}
	if (Side == FStatic::Back && BuildBlock -> Back) {
		return true;
	}
	if (Side == FStatic::Right && BuildBlock -> Right) {
		return true;
	}
	if (Side == FStatic::Low && BuildBlock -> Low) {
		return true;
	}
	if (Side == FStatic::Left && BuildBlock -> Left) {
		return true;
	}
	if (Side == FStatic::Up && BuildBlock -> Up) {
		return true;
	}
	return false;
}

bool FStr::IsWallSideBlock(const FString Side, const FBuildings* BuildBlock)
{
	if (BuildBlock->Type.Foundation) {
		if (Side == FStatic::Right && BuildBlock -> WallRight) {
			return true;
		}
		if (Side == FStatic::Low && BuildBlock -> WallLow) {
			return true;
		}
		if (Side == FStatic::Left && BuildBlock -> WallLeft) {
			return true;
		}
		if (Side == FStatic::Up && BuildBlock -> WallUp) {
			return true;
		}
	}
	if (BuildBlock->Type.Wall) {
		if (Side == FStatic::Low && BuildBlock -> Low) {
			return true;
		}
		if (Side == FStatic::Up && BuildBlock -> Up) {
			return true;
		}
	}
	return false;
}

bool FStr::IsContain(const FString Value, const FString Key)
{
	return Value . Contains(Key);
}


FBlockActor FStr::SetSide(const FString Key, FBlockActor Side, const bool IsRemove)
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
		if (Key == FStatic::DownWallRight) {
			Side . DownWallRight = true;
		}
		if (Key == FStatic::DownWallLow) {
			Side . DownWallLow = true;
		}
		if (Key == FStatic::DownWallLeft) {
			Side . DownWallLeft = true;
		}
		if (Key == FStatic::DownWallUp) {
			Side . DownWallUp = true;
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
		if (Key == FStatic::DownWallRight) {
			Side . DownWallRight = false;
		}
		if (Key == FStatic::DownWallLow) {
			Side . DownWallLow = false;
		}
		if (Key == FStatic::DownWallLeft) {
			Side . DownWallLeft = false;
		}
		if (Key == FStatic::DownWallUp) {
			Side . DownWallUp = false;
		}
	}
	return Side;
}

FBuildings FStr::SetSide(const FString Key, FBuildings Side, const bool IsRemove, const bool IsWall)
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
