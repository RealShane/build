// ReSharper disable All
#include "BuildSystem.h"

UBuildSystem::UBuildSystem()
{
	PrimaryComponentTick . bCanEverTick = true;
	BuildDistance = FStatic::FiveHundred;
}

void UBuildSystem::BeginPlay()
{
	Super::BeginPlay();
}

void UBuildSystem::TickComponent(float DeltaTime, ELevelTick TickType,
                                 FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	this -> BlurAttach();
}

void UBuildSystem::SetPlayer(AMainCharacter* Value)
{
	this -> Player = Value;
}

bool UBuildSystem::Building()
{
	if (BuildType == FStatic::Foundation) {
		return this -> FoundationBuild();
	}
	if (BuildType == FStatic::Wall) {
		return this -> WallBuild();
	}
	if (BuildType == FStatic::Floor) {
		return this -> FloorBuild();
	}
	return false;
}

/**
 * 显示取消虚影
 */
void UBuildSystem::SetBuild(const FString Type)
{
	if (Type == FStatic::Foundation) {
		this -> Foundation();
	} else if (Type == FStatic::Wall) {
		this -> Wall();
	} else if (Type == FStatic::Floor) {
		this -> Floor();
	}
}

void UBuildSystem::UnSetBuild()
{
	if (BuildType == FStatic::Foundation) {
		GetWorld() -> DestroyActor(FoundationBase);
	}
	if (BuildType == FStatic::Wall) {
		GetWorld() -> DestroyActor(WallBase);
	}
	if (BuildType == FStatic::Floor) {
		GetWorld() -> DestroyActor(FloorBase);
	}
	FoundationBase = nullptr;
	WallBase = nullptr;
	FloorBase = nullptr;
	BuildType = nullptr;
}

/**
 * 虚影可移动附着
 */
void UBuildSystem::BlurAttach()
{
	if (BuildType == FStatic::Foundation) {
		this -> FoundationBlurAttach();
	} else if (BuildType == FStatic::Wall) {
		this -> WallBlurAttach();
	} else if (BuildType == FStatic::Floor) {
		this -> FloorBlurAttach();
	}
}

bool UBuildSystem::IsBuildMode() const
{
	return !BuildType . IsEmpty();
}

void UBuildSystem::Foundation()
{
	if (BuildType != FStatic::Foundation) {
		this -> UnSetBuild();
	}
	if (FoundationBase == nullptr) {
		FoundationBase = GetWorld() -> SpawnActor<AFoundation>(
			FVector(FStatic::Zero, FStatic::Zero, FStatic::FiveThousand),
			FRotator(FStatic::Zero));
		FoundationBase -> SetCollision(ECollisionEnabled::QueryOnly);
		BuildType = FStatic::Foundation;
	} else {
		this -> UnSetBuild();
	}
}

void UBuildSystem::FoundationBlurAttach()
{
	if (FoundationBase != nullptr) {
		//视角旋转的Yaw值是极坐标ρ
		const FRotator ViewRotation = Player -> GetController() -> GetControlRotation();
		const FVector MainLocation = Player -> GetActorLocation();
		float BuildRotation = ViewRotation . Yaw;
		//ρ值有一个变化区间可以使建筑前后移动，这个变化区间值的求出应当使用人物与摄像机的夹角 z*tan
		float Angle = FStatic::Ninety - (FStatic::ThreeHundredAndSixty - ViewRotation . Pitch);
		if (Angle <= FStatic::Zero) {
			Angle = FStatic::EightyNine;
		}
		BuildDistance = FMath::Tan(FMath::DegreesToRadians(Angle)) * MainLocation . Z;
		if (BuildDistance > FStatic::Thousand) {
			BuildDistance = FStatic::Thousand;
		}
		/**
		 * x = ρcos
		 * y = ρsin
		 */
		const float X = FMath::Cos(FMath::DegreesToRadians(ViewRotation . Yaw)) * BuildDistance;
		const float Y = FMath::Sin(FMath::DegreesToRadians(ViewRotation . Yaw)) * BuildDistance;
		float Z = FoundationBase -> LandZ;
		if (Z <= FStatic::Zero) {
			Z = Player -> GetActorLocation() . Z;
		}
		BuildLocation = FVector(MainLocation . X, MainLocation . Y, FStatic::Zero) + FVector(X, Y, Z);
		// 光线击中处附着
		const bool IsHit = Player -> RayHit . IsValidBlockingHit();
		if (IsHit) {
			const FString HitName = Player -> RayHit . GetActor() -> GetName();
			const bool IsFoundation = FStr::IsContain(HitName, FStatic::Foundation);
			if (IsFoundation) {
				const FBuildings BuildBlock = Buildings[HitName];
				const FString AttachSide = Player -> RayHit . GetComponent() -> GetName();
				const float Side = FoundationBase -> HalfXY * FStatic::Two;
				if (AttachSide == FStatic::Right && !BuildBlock . Right) {
					const float CalX = FMath::Cos(
							FMath::DegreesToRadians(BuildBlock . Rotation . Yaw + FStatic::Ninety)) *
						Side;
					const float CalY = FMath::Sin(
							FMath::DegreesToRadians(BuildBlock . Rotation . Yaw + FStatic::Ninety)) *
						Side;
					BuildLocation = BuildBlock . Location + FVector(CalX, CalY, FStatic::Zero);
					BuildRotation = BuildBlock . Rotation . Yaw;
				}
				if (AttachSide == FStatic::Low && !BuildBlock . Low) {
					const float CalX = FMath::Cos(
						FMath::DegreesToRadians(BuildBlock . Rotation . Yaw + FStatic::HundredAndEighty)) * Side;
					const float CalY = FMath::Sin(
						FMath::DegreesToRadians(BuildBlock . Rotation . Yaw + FStatic::HundredAndEighty)) * Side;
					BuildLocation = BuildBlock . Location + FVector(CalX, CalY, FStatic::Zero);
					BuildRotation = BuildBlock . Rotation . Yaw;
				}
				if (AttachSide == FStatic::Left && !BuildBlock . Left) {
					const float CalX = FMath::Cos(
						FMath::DegreesToRadians(BuildBlock . Rotation . Yaw + FStatic::TwoHundredAndSeventy)) * Side;
					const float CalY = FMath::Sin(
						FMath::DegreesToRadians(BuildBlock . Rotation . Yaw + FStatic::TwoHundredAndSeventy)) * Side;
					BuildLocation = BuildBlock . Location + FVector(CalX, CalY, FStatic::Zero);
					BuildRotation = BuildBlock . Rotation . Yaw;
				}
				if (AttachSide == FStatic::Up && !BuildBlock . Up) {
					const float CalX = FMath::Cos(
						FMath::DegreesToRadians(BuildBlock . Rotation . Yaw + FStatic::ThreeHundredAndSixty)) * Side;
					const float CalY = FMath::Sin(
						FMath::DegreesToRadians(BuildBlock . Rotation . Yaw + FStatic::ThreeHundredAndSixty)) * Side;
					BuildLocation = BuildBlock . Location + FVector(CalX, CalY, FStatic::Zero);
					BuildRotation = BuildBlock . Rotation . Yaw;
				}
			}
		}
		FoundationBase -> SetActorRotation(FRotator(FStatic::Zero, BuildRotation, FStatic::Zero));
		FoundationBase -> SetActorLocation(BuildLocation);
	}
}

bool UBuildSystem::FoundationBuild()
{
	if (FoundationBase == nullptr) {
		return false;
	}
	if (FoundationBase -> IsBlock) {
		//TODO UI显示被阻挡无法放置
		FLib::Echo(TEXT("被阻挡无法放置！"));
		return false;
	}
	FoundationBase -> StaticMeshComponent -> SetMobility(EComponentMobility::Static);
	FoundationBase -> StaticMeshComponent -> SetCollisionObjectType(ECC_WorldStatic);
	FoundationBase -> BoxComponent -> SetCollisionObjectType(ECC_WorldStatic);
	FoundationBase -> RightSideBoxComponent -> SetCollisionObjectType(ECC_WorldStatic);
	FoundationBase -> LowSideBoxComponent -> SetCollisionObjectType(ECC_WorldStatic);
	FoundationBase -> LeftSideBoxComponent -> SetCollisionObjectType(ECC_WorldStatic);
	FoundationBase -> UpSideBoxComponent -> SetCollisionObjectType(ECC_WorldStatic);
	FoundationBase -> SetCollision(ECollisionEnabled::QueryAndPhysics);
	FoundationBase -> SetMaterial(FStatic::WoodMaterial);
	FoundationBase -> PrimaryActorTick . bCanEverTick = false;
	FoundationBase -> IsSet = true;
	FBuildings Cache;
	Cache . HealthPoints = FStatic::Hundred;
	Cache . Type . Foundation = true;
	Cache . Building = FoundationBase;
	Cache . Location = BuildLocation;
	Cache . Rotation = FoundationBase -> GetActorRotation();
	if (FoundationBase -> IsAttach) {
		for (auto& Item : FoundationBase -> BlockSideCache) {
			if (Item . Value . Right) {
				Buildings[Item . Key] . Right = Item . Value . Right;
			}
			if (Item . Value . Low) {
				Buildings[Item . Key] . Low = Item . Value . Low;
			}
			if (Item . Value . Left) {
				Buildings[Item . Key] . Left = Item . Value . Left;
			}
			if (Item . Value . Up) {
				Buildings[Item . Key] . Up = Item . Value . Up;
			}
		}
		Cache . Right = FoundationBase -> Right;
		Cache . Low = FoundationBase -> Low;
		Cache . Left = FoundationBase -> Left;
		Cache . Up = FoundationBase -> Up;
		Cache . WallRight = FoundationBase -> WallRight;
		Cache . WallLow = FoundationBase -> WallLow;
		Cache . WallLeft = FoundationBase -> WallLeft;
		Cache . WallUp = FoundationBase -> WallUp;
	}
	Buildings . Emplace(FoundationBase -> GetName(), Cache);
	BuildType = nullptr;
	FoundationBase = nullptr;
	return true;
}


void UBuildSystem::Wall()
{
	if (BuildType != FStatic::Wall) {
		this -> UnSetBuild();
	}
	if (WallBase == nullptr) {
		WallBase = GetWorld() -> SpawnActor<AWall>(FVector(FStatic::Zero, FStatic::Zero, FStatic::FiveThousand),
		                                           FRotator(FStatic::Zero));
		WallBase -> SetCollision(ECollisionEnabled::QueryOnly);
		BuildType = FStatic::Wall;
	} else {
		this -> UnSetBuild();
	}
}

void UBuildSystem::WallBlurAttach()
{
	if (WallBase != nullptr) {
		//视角旋转的Yaw值是极坐标ρ
		const FRotator ViewRotation = Player -> GetController() -> GetControlRotation();
		const FVector MainLocation = Player -> GetActorLocation();
		float BuildRotation = ViewRotation . Yaw;
		//ρ值有一个变化区间可以使建筑前后移动，这个变化区间值的求出应当使用人物与摄像机的夹角 z*tan
		const float OriginAngle = FStatic::Ninety - (FStatic::ThreeHundredAndSixty - ViewRotation . Pitch);
		float Angle = OriginAngle;
		if (Angle <= FStatic::Zero) {
			Angle = FStatic::EightyNine;
		}
		BuildDistance = FMath::Tan(FMath::DegreesToRadians(Angle)) * MainLocation . Z;
		if (BuildDistance > FStatic::Thousand) {
			BuildDistance = FStatic::Thousand;
		}
		/**
		 * x = ρcos
		 * y = ρsin
		 */
		float X = FMath::Cos(FMath::DegreesToRadians(ViewRotation . Yaw)) * BuildDistance;
		float Y = FMath::Sin(FMath::DegreesToRadians(ViewRotation . Yaw)) * BuildDistance;
		float Z = WallBase -> LandZ;
		if (OriginAngle < -FStatic::HundredAndEighty && OriginAngle > -FStatic::TwoHundredAndSeventy) {
			Z = FMath::Tan(FMath::DegreesToRadians(OriginAngle + FStatic::TwoHundredAndSeventy)) *
				FStatic::ThousandAndTwoHundred;
			if (Z > FStatic::Thousand) {
				Z = FStatic::Thousand;
			}
			X = FMath::Cos(FMath::DegreesToRadians(ViewRotation . Yaw)) * (FStatic::ThousandOne - Z);
			Y = FMath::Sin(FMath::DegreesToRadians(ViewRotation . Yaw)) * (FStatic::ThousandOne - Z);
		}
		BuildLocation = FVector(MainLocation . X, MainLocation . Y, FStatic::Hundred) + FVector(X, Y, Z);
		// 光线击中处附着
		const bool IsHit = Player -> RayHit . IsValidBlockingHit();
		if (IsHit) {
			const FString HitName = Player -> RayHit . GetActor() -> GetName();
			const bool IsFoundation = FStr::IsContain(HitName, FStatic::Foundation);
			const bool IsWall = FStr::IsContain(HitName, FStatic::Wall);
			const bool IsFloor = FStr::IsContain(HitName, FStatic::Floor);
			const float Side = WallBase -> HalfYZ;
			FString AttachSide = Player -> RayHit . GetComponent() -> GetName();
			if (IsFoundation || IsFloor) {
				float CalZ = FStatic::Zero;
				if (IsFoundation) {
					CalZ = FStatic::Thirty;
				}
				if (IsFloor) {
					CalZ = FStatic::Four;
				}
				const FBuildings BuildBlock = Buildings[HitName];
				if (AttachSide == FStatic::Right && !BuildBlock . WallRight) {
					const float CalX = FMath::Cos(
						FMath::DegreesToRadians(BuildBlock . Rotation . Yaw + FStatic::Ninety)) * Side;
					const float CalY = FMath::Sin(
						FMath::DegreesToRadians(BuildBlock . Rotation . Yaw + FStatic::Ninety)) * Side;
					BuildLocation = BuildBlock . Location + FVector(CalX, CalY, CalZ + Side);
					BuildRotation = BuildBlock . Rotation . Yaw + FStatic::Ninety;
				}
				if (AttachSide == FStatic::Low && !BuildBlock . WallLow) {
					const float CalX = FMath::Cos(
						FMath::DegreesToRadians(BuildBlock . Rotation . Yaw + FStatic::HundredAndEighty)) * Side;
					const float CalY = FMath::Sin(
						FMath::DegreesToRadians(BuildBlock . Rotation . Yaw + FStatic::HundredAndEighty)) * Side;
					BuildLocation = BuildBlock . Location + FVector(CalX, CalY, CalZ + Side);
					BuildRotation = BuildBlock . Rotation . Yaw + FStatic::HundredAndEighty;
				}
				if (AttachSide == FStatic::Left && !BuildBlock . WallLeft) {
					const float CalX = FMath::Cos(
						FMath::DegreesToRadians(BuildBlock . Rotation . Yaw + FStatic::TwoHundredAndSeventy)) * Side;
					const float CalY = FMath::Sin(
						FMath::DegreesToRadians(BuildBlock . Rotation . Yaw + FStatic::TwoHundredAndSeventy)) * Side;
					BuildLocation = BuildBlock . Location + FVector(CalX, CalY, CalZ + Side);
					BuildRotation = BuildBlock . Rotation . Yaw + FStatic::TwoHundredAndSeventy;
				}
				if (AttachSide == FStatic::Up && !BuildBlock . WallUp) {
					const float CalX = FMath::Cos(
						FMath::DegreesToRadians(BuildBlock . Rotation . Yaw + FStatic::ThreeHundredAndSixty)) * Side;
					const float CalY = FMath::Sin(
						FMath::DegreesToRadians(BuildBlock . Rotation . Yaw + FStatic::ThreeHundredAndSixty)) * Side;
					BuildLocation = BuildBlock . Location + FVector(CalX, CalY, CalZ + Side);
					BuildRotation = BuildBlock . Rotation . Yaw + FStatic::ThreeHundredAndSixty;
				}

				if (AttachSide == FStatic::DownWallRight && !BuildBlock . DownWallRight) {
					const float CalX = FMath::Cos(
						FMath::DegreesToRadians(BuildBlock . Rotation . Yaw + FStatic::Ninety)) * Side;
					const float CalY = FMath::Sin(
						FMath::DegreesToRadians(BuildBlock . Rotation . Yaw + FStatic::Ninety)) * Side;
					BuildLocation = BuildBlock . Location + FVector(CalX, CalY, -(CalZ + Side));
					BuildRotation = BuildBlock . Rotation . Yaw + FStatic::Ninety;
				}
				if (AttachSide == FStatic::DownWallLow && !BuildBlock . DownWallLow) {
					const float CalX = FMath::Cos(
						FMath::DegreesToRadians(BuildBlock . Rotation . Yaw + FStatic::HundredAndEighty)) * Side;
					const float CalY = FMath::Sin(
						FMath::DegreesToRadians(BuildBlock . Rotation . Yaw + FStatic::HundredAndEighty)) * Side;
					BuildLocation = BuildBlock . Location + FVector(CalX, CalY, -(CalZ + Side));
					BuildRotation = BuildBlock . Rotation . Yaw + FStatic::HundredAndEighty;
				}
				if (AttachSide == FStatic::DownWallLeft && !BuildBlock . DownWallLeft) {
					const float CalX = FMath::Cos(
						FMath::DegreesToRadians(BuildBlock . Rotation . Yaw + FStatic::TwoHundredAndSeventy)) * Side;
					const float CalY = FMath::Sin(
						FMath::DegreesToRadians(BuildBlock . Rotation . Yaw + FStatic::TwoHundredAndSeventy)) * Side;
					BuildLocation = BuildBlock . Location + FVector(CalX, CalY, -(CalZ + Side));
					BuildRotation = BuildBlock . Rotation . Yaw + FStatic::TwoHundredAndSeventy;
				}
				if (AttachSide == FStatic::DownWallUp && !BuildBlock . DownWallUp) {
					const float CalX = FMath::Cos(
						FMath::DegreesToRadians(BuildBlock . Rotation . Yaw + FStatic::ThreeHundredAndSixty)) * Side;
					const float CalY = FMath::Sin(
						FMath::DegreesToRadians(BuildBlock . Rotation . Yaw + FStatic::ThreeHundredAndSixty)) * Side;
					BuildLocation = BuildBlock . Location + FVector(CalX, CalY, -(CalZ + Side));
					BuildRotation = BuildBlock . Rotation . Yaw + FStatic::ThreeHundredAndSixty;
				}
			}
			if (IsWall) {
				const FBuildings BuildBlock = Buildings[HitName];
				if (AttachSide == FStatic::Low && !BuildBlock . Low) {
					BuildLocation = BuildBlock . Location +
						FVector(FStatic::Zero, FStatic::Zero, -Side * FStatic::Two);
					BuildRotation = BuildBlock . Rotation . Yaw;
				}
				if (AttachSide == FStatic::Up && !BuildBlock . Up) {
					BuildLocation = BuildBlock . Location +
						FVector(FStatic::Zero, FStatic::Zero, Side * FStatic::Two);
					BuildRotation = BuildBlock . Rotation . Yaw;
				}
			}
		}
		WallBase -> SetActorRotation(FRotator(FStatic::Zero, BuildRotation, FStatic::Zero));
		WallBase -> SetActorLocation(BuildLocation);
	}
}

bool UBuildSystem::WallBuild()
{
	if (WallBase == nullptr) {
		return false;
	}
	if (WallBase -> IsBlock) {
		//TODO UI显示被阻挡无法放置
		FLib::Echo(TEXT("被阻挡无法放置！"));
		return false;
	}
	if (!WallBase -> IsAttach) {
		//TODO UI显示被阻挡无法放置
		FLib::Echo(TEXT("未附着无法放置！"));
		return false;
	}
	WallBase -> StaticMeshComponent -> SetMobility(EComponentMobility::Stationary);
	WallBase -> StaticMeshComponent -> SetCollisionObjectType(ECC_WorldStatic);
	WallBase -> BoxComponent -> SetCollisionObjectType(ECC_WorldStatic);
	WallBase -> FrontBoxComponent -> SetCollisionObjectType(ECC_WorldStatic);
	WallBase -> BackBoxComponent -> SetCollisionObjectType(ECC_WorldStatic);
	WallBase -> LowSideBoxComponent -> SetCollisionObjectType(ECC_WorldStatic);
	WallBase -> UpSideBoxComponent -> SetCollisionObjectType(ECC_WorldStatic);
	WallBase -> SetCollision(ECollisionEnabled::QueryAndPhysics);
	WallBase -> SetMaterial(FStatic::WoodMaterial);
	WallBase -> PrimaryActorTick . bCanEverTick = false;
	WallBase -> IsSet = true;
	FBuildings Cache;
	Cache . HealthPoints = FStatic::Hundred;
	Cache . Type . Wall = true;
	Cache . Building = WallBase;
	Cache . Location = BuildLocation;
	Cache . Rotation = WallBase -> GetActorRotation();
	for (auto& Item : WallBase -> BlockSideCache) {
		if (Buildings[Item . Key] . Type . Foundation || Buildings[Item . Key] . Type . Floor) {
			if (Item . Value . Right) {
				Buildings[Item . Key] . WallRight = Item . Value . Right;
			}
			if (Item . Value . Low) {
				Buildings[Item . Key] . WallLow = Item . Value . Low;
			}
			if (Item . Value . Left) {
				Buildings[Item . Key] . WallLeft = Item . Value . Left;
			}
			if (Item . Value . Up) {
				Buildings[Item . Key] . WallUp = Item . Value . Up;
			}
			if (Item . Value . DownWallRight) {
				Buildings[Item . Key] . DownWallRight = Item . Value . DownWallRight;
			}
			if (Item . Value . DownWallLow) {
				Buildings[Item . Key] . DownWallLow = Item . Value . DownWallLow;
			}
			if (Item . Value . DownWallLeft) {
				Buildings[Item . Key] . DownWallLeft = Item . Value . DownWallLeft;
			}
			if (Item . Value . DownWallUp) {
				Buildings[Item . Key] . DownWallUp = Item . Value . DownWallUp;
			}
		} else {
			if (Item . Value . Low) {
				Buildings[Item . Key] . Low = Item . Value . Low;
			} else if (Item . Value . Up) {
				Buildings[Item . Key] . Up = Item . Value . Up;
			}
		}
	}
	Cache . Front = WallBase -> Front;
	Cache . Back = WallBase -> Back;
	Cache . Low = WallBase -> Low;
	Cache . Up = WallBase -> Up;
	Buildings . Emplace(WallBase -> GetName(), Cache);
	BuildType = nullptr;
	WallBase = nullptr;
	return true;
}

void UBuildSystem::Floor()
{
	if (BuildType != FStatic::Floor) {
		this -> UnSetBuild();
	}
	if (FloorBase == nullptr) {
		FloorBase = GetWorld() -> SpawnActor<AFloor>(FVector(FStatic::Zero, FStatic::Zero, FStatic::FiveThousand),
		                                             FRotator(FStatic::Zero));
		FloorBase -> SetCollision(ECollisionEnabled::QueryOnly);

		BuildType = FStatic::Floor;
	} else {
		this -> UnSetBuild();
	}
}

void UBuildSystem::FloorBlurAttach()
{
	if (FloorBase != nullptr) {
		//视角旋转的Yaw值是极坐标ρ
		const FRotator ViewRotation = Player -> GetController() -> GetControlRotation();
		const FVector MainLocation = Player -> GetActorLocation();
		float BuildRotation = ViewRotation . Yaw;
		//ρ值有一个变化区间可以使建筑前后移动，这个变化区间值的求出应当使用人物与摄像机的夹角 z*tan
		const float OriginAngle = FStatic::Ninety - (FStatic::ThreeHundredAndSixty - ViewRotation . Pitch);
		float Angle = OriginAngle;
		if (Angle <= FStatic::Zero) {
			Angle = FStatic::EightyNine;
		}
		BuildDistance = FMath::Tan(FMath::DegreesToRadians(Angle)) * MainLocation . Z;
		if (BuildDistance > FStatic::Thousand) {
			BuildDistance = FStatic::Thousand;
		}
		/**
		 * x = ρcos
		 * y = ρsin
		 */
		float X = FMath::Cos(FMath::DegreesToRadians(ViewRotation . Yaw)) * BuildDistance;
		float Y = FMath::Sin(FMath::DegreesToRadians(ViewRotation . Yaw)) * BuildDistance;
		float Z = FloorBase -> LandZ;
		if (OriginAngle < -FStatic::HundredAndEighty && OriginAngle > -FStatic::TwoHundredAndSeventy) {
			Z = FMath::Tan(FMath::DegreesToRadians(OriginAngle + FStatic::TwoHundredAndSeventy)) * FStatic::FiveHundred;
			if (Z > FStatic::Thousand) {
				Z = FStatic::Thousand;
			}
			X = FMath::Cos(FMath::DegreesToRadians(ViewRotation . Yaw)) * (FStatic::ThousandOne - Z);
			Y = FMath::Sin(FMath::DegreesToRadians(ViewRotation . Yaw)) * (FStatic::ThousandOne - Z);
		}
		BuildLocation = FVector(MainLocation . X, MainLocation . Y, FStatic::Zero) + FVector(X, Y, Z);
		// 光线击中处附着
		const bool IsHit = Player -> RayHit . IsValidBlockingHit();
		if (IsHit) {
			const FString HitName = Player -> RayHit . GetActor() -> GetName();
			const bool IsFoundation = FStr::IsContain(HitName, FStatic::Foundation);
			const bool IsWall = FStr::IsContain(HitName, FStatic::Wall);
			const bool IsFloor = FStr::IsContain(HitName, FStatic::Floor);
			FString AttachSide = Player -> RayHit . GetComponent() -> GetName();
			if (IsFoundation || IsFloor) {
				const float Side = FloorBase -> HalfXY * FStatic::Two;
				float CalZ = FStatic::Zero;
				if (IsFoundation) {
					CalZ = FStatic::TwentyFive;
				}
				if (IsFloor) {
					CalZ = FStatic::Zero;
				}
				const FBuildings BuildBlock = Buildings[HitName];
				if (AttachSide == FStatic::Right && !BuildBlock . Right) {
					const float CalX = FMath::Cos(
							FMath::DegreesToRadians(BuildBlock . Rotation . Yaw + FStatic::Ninety)) *
						Side;
					const float CalY = FMath::Sin(
							FMath::DegreesToRadians(BuildBlock . Rotation . Yaw + FStatic::Ninety)) *
						Side;
					BuildLocation = BuildBlock . Location + FVector(CalX, CalY, CalZ);
					BuildRotation = BuildBlock . Rotation . Yaw;
				}
				if (AttachSide == FStatic::Low && !BuildBlock . Low) {
					const float CalX = FMath::Cos(
						FMath::DegreesToRadians(BuildBlock . Rotation . Yaw + FStatic::HundredAndEighty)) * Side;
					const float CalY = FMath::Sin(
						FMath::DegreesToRadians(BuildBlock . Rotation . Yaw + FStatic::HundredAndEighty)) * Side;
					BuildLocation = BuildBlock . Location + FVector(CalX, CalY, CalZ);
					BuildRotation = BuildBlock . Rotation . Yaw;
				}
				if (AttachSide == FStatic::Left && !BuildBlock . Left) {
					const float CalX = FMath::Cos(
						FMath::DegreesToRadians(BuildBlock . Rotation . Yaw + FStatic::TwoHundredAndSeventy)) * Side;
					const float CalY = FMath::Sin(
						FMath::DegreesToRadians(BuildBlock . Rotation . Yaw + FStatic::TwoHundredAndSeventy)) * Side;
					BuildLocation = BuildBlock . Location + FVector(CalX, CalY, CalZ);
					BuildRotation = BuildBlock . Rotation . Yaw;
				}
				if (AttachSide == FStatic::Up && !BuildBlock . Up) {
					const float CalX = FMath::Cos(
						FMath::DegreesToRadians(BuildBlock . Rotation . Yaw + FStatic::ThreeHundredAndSixty)) * Side;
					const float CalY = FMath::Sin(
						FMath::DegreesToRadians(BuildBlock . Rotation . Yaw + FStatic::ThreeHundredAndSixty)) * Side;
					BuildLocation = BuildBlock . Location + FVector(CalX, CalY, CalZ);
					BuildRotation = BuildBlock . Rotation . Yaw;
				}
				if (AttachSide == FStatic::DownWallRight && !BuildBlock . Right) {
					const float CalX = FMath::Cos(
							FMath::DegreesToRadians(BuildBlock . Rotation . Yaw + FStatic::Ninety)) *
						Side;
					const float CalY = FMath::Sin(
							FMath::DegreesToRadians(BuildBlock . Rotation . Yaw + FStatic::Ninety)) *
						Side;
					BuildLocation = BuildBlock . Location + FVector(CalX, CalY, CalZ);
					BuildRotation = BuildBlock . Rotation . Yaw;
				}
				if (AttachSide == FStatic::DownWallLow && !BuildBlock . Low) {
					const float CalX = FMath::Cos(
						FMath::DegreesToRadians(BuildBlock . Rotation . Yaw + FStatic::HundredAndEighty)) * Side;
					const float CalY = FMath::Sin(
						FMath::DegreesToRadians(BuildBlock . Rotation . Yaw + FStatic::HundredAndEighty)) * Side;
					BuildLocation = BuildBlock . Location + FVector(CalX, CalY, CalZ);
					BuildRotation = BuildBlock . Rotation . Yaw;
				}
				if (AttachSide == FStatic::DownWallLeft && !BuildBlock . Left) {
					const float CalX = FMath::Cos(
						FMath::DegreesToRadians(BuildBlock . Rotation . Yaw + FStatic::TwoHundredAndSeventy)) * Side;
					const float CalY = FMath::Sin(
						FMath::DegreesToRadians(BuildBlock . Rotation . Yaw + FStatic::TwoHundredAndSeventy)) * Side;
					BuildLocation = BuildBlock . Location + FVector(CalX, CalY, CalZ);
					BuildRotation = BuildBlock . Rotation . Yaw;
				}
				if (AttachSide == FStatic::DownWallUp && !BuildBlock . Up) {
					const float CalX = FMath::Cos(
						FMath::DegreesToRadians(BuildBlock . Rotation . Yaw + FStatic::ThreeHundredAndSixty)) * Side;
					const float CalY = FMath::Sin(
						FMath::DegreesToRadians(BuildBlock . Rotation . Yaw + FStatic::ThreeHundredAndSixty)) * Side;
					BuildLocation = BuildBlock . Location + FVector(CalX, CalY, CalZ);
					BuildRotation = BuildBlock . Rotation . Yaw;
				}
			}
			if (IsWall) {
				const float Side = FloorBase -> HalfXY;
				const FBuildings BuildBlock = Buildings[HitName];
				if (AttachSide == FStatic::Front && !BuildBlock . Front) {
					const float CalX = FMath::Cos(
						FMath::DegreesToRadians(BuildBlock . Rotation . Yaw + FStatic::HundredAndEighty)) * Side;
					const float CalY = FMath::Sin(
						FMath::DegreesToRadians(BuildBlock . Rotation . Yaw + FStatic::HundredAndEighty)) * Side;
					BuildLocation = BuildBlock . Location + FVector(CalX, CalY, FStatic::TwoHundredAndFour);
					BuildRotation = BuildBlock . Rotation . Yaw;
				}
				if (AttachSide == FStatic::Back && !BuildBlock . Back) {
					const float CalX = FMath::Cos(
						FMath::DegreesToRadians(BuildBlock . Rotation . Yaw + FStatic::ThreeHundredAndSixty)) * Side;
					const float CalY = FMath::Sin(
						FMath::DegreesToRadians(BuildBlock . Rotation . Yaw + FStatic::ThreeHundredAndSixty)) * Side;
					BuildLocation = BuildBlock . Location + FVector(CalX, CalY, FStatic::TwoHundredAndFour);
					BuildRotation = BuildBlock . Rotation . Yaw;
				}
			}
		}
		FloorBase -> SetActorRotation(FRotator(FStatic::Zero, BuildRotation, FStatic::Zero));
		FloorBase -> SetActorLocation(BuildLocation);
	}
}

bool UBuildSystem::FloorBuild()
{
	if (FloorBase == nullptr) {
		return false;
	}
	if (FloorBase -> IsBlock) {
		//TODO UI显示被阻挡无法放置
		FLib::Echo(TEXT("被阻挡无法放置！"));
		return false;
	}
	if (!FloorBase -> IsAttach) {
		//TODO UI显示被阻挡无法放置
		FLib::Echo(TEXT("未附着无法放置！"));
		return false;
	}
	FloorBase -> StaticMeshComponent -> SetMobility(EComponentMobility::Stationary);
	FloorBase -> StaticMeshComponent -> SetCollisionObjectType(ECC_WorldStatic);
	FloorBase -> BoxComponent -> SetCollisionObjectType(ECC_WorldStatic);
	FloorBase -> RightSideBoxComponent -> SetCollisionObjectType(ECC_WorldStatic);
	FloorBase -> LowSideBoxComponent -> SetCollisionObjectType(ECC_WorldStatic);
	FloorBase -> LeftSideBoxComponent -> SetCollisionObjectType(ECC_WorldStatic);
	FloorBase -> UpSideBoxComponent -> SetCollisionObjectType(ECC_WorldStatic);
	FloorBase -> DownRightSideBoxComponent -> SetCollisionObjectType(ECC_WorldStatic);
	FloorBase -> DownLowSideBoxComponent -> SetCollisionObjectType(ECC_WorldStatic);
	FloorBase -> DownLeftSideBoxComponent -> SetCollisionObjectType(ECC_WorldStatic);
	FloorBase -> DownUpSideBoxComponent -> SetCollisionObjectType(ECC_WorldStatic);
	FloorBase -> SetCollision(ECollisionEnabled::QueryAndPhysics);
	FloorBase -> SetMaterial(FStatic::WoodMaterial);
	FloorBase -> PrimaryActorTick . bCanEverTick = false;
	FloorBase -> IsSet = true;
	FBuildings Cache;
	Cache . HealthPoints = FStatic::Hundred;
	Cache . Type . Floor = true;
	Cache . Building = FloorBase;
	Cache . Location = BuildLocation;
	Cache . Rotation = FloorBase -> GetActorRotation();
	for (auto& Item : FloorBase -> BlockSideCache) {
		if (Buildings[Item . Key] . Type . Foundation || Buildings[Item . Key] . Type . Floor) {
			if (Item . Value . Right) {
				Buildings[Item . Key] . Right = Item . Value . Right;
			}
			if (Item . Value . Low) {
				Buildings[Item . Key] . Low = Item . Value . Low;
			}
			if (Item . Value . Left) {
				Buildings[Item . Key] . Left = Item . Value . Left;
			}
			if (Item . Value . Up) {
				Buildings[Item . Key] . Up = Item . Value . Up;
			}
		} else {
			if (Item . Value . Front) {
				Buildings[Item . Key] . Front = Item . Value . Front;
			} else if (Item . Value . Back) {
				Buildings[Item . Key] . Back = Item . Value . Back;
			}
		}
	}
	Cache . Right = FloorBase -> Right;
	Cache . Low = FloorBase -> Low;
	Cache . Left = FloorBase -> Left;
	Cache . Up = FloorBase -> Up;
	Cache . WallRight = FloorBase -> WallRight;
	Cache . WallLow = FloorBase -> WallLow;
	Cache . WallLeft = FloorBase -> WallLeft;
	Cache . WallUp = FloorBase -> WallUp;
	Cache . DownWallRight = FloorBase -> DownWallRight;
	Cache . DownWallLow = FloorBase -> DownWallLow;
	Cache . DownWallLeft = FloorBase -> DownWallLeft;
	Cache . DownWallUp = FloorBase -> DownWallUp;
	Buildings . Emplace(FloorBase -> GetName(), Cache);
	BuildType = nullptr;
	FloorBase = nullptr;
	return true;
}
