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
		GetWorld() -> DestroyActor(Cast<AFoundation>(BuildItem));
	} else if (BuildType == FStatic::Wall) {
		GetWorld() -> DestroyActor(Cast<AWall>(BuildItem));
	} else if (BuildType == FStatic::Floor) {
		GetWorld() -> DestroyActor(Cast<AFloor>(BuildItem));
	}
	BuildItem = nullptr;
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
	return BuildItem != nullptr;
}

void UBuildSystem::Foundation()
{
	if (BuildType != FStatic::Foundation) {
		this -> UnSetBuild();
	}
	if (BuildItem == nullptr) {
		BuildItem = GetWorld() -> SpawnActor<AFoundation>(FVector(FStatic::Zero, FStatic::Zero, FStatic::FiveThousand),
		                                                  FRotator(FStatic::Zero));
		Cast<AFoundation>(BuildItem) -> SetCollision(ECollisionEnabled::QueryOnly);
		BuildType = FStatic::Foundation;
	} else {
		this -> UnSetBuild();
	}
}

void UBuildSystem::FoundationBlurAttach()
{
	if (BuildItem != nullptr) {
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
		float Z = Cast<AFoundation>(BuildItem) -> LandZ;
		if (Z <= FStatic::Zero) {
			Z = Cast<AFoundation>(BuildItem) -> GetActorLocation() . Z;
		}
		BuildLocation = FVector(MainLocation . X, MainLocation . Y, FStatic::Zero) + FVector(X, Y, Z);
		// 检测阻挡建筑是否可以附着
		if (Cast<AFoundation>(BuildItem) -> IsAttach) {
			const FString BlockActorName = Cast<AFoundation>(BuildItem) -> BlockActorName;
			const FVector BlockActorLocation = Saving[BlockActorName] . Location;
			const FRotator BlockActorRotation = Saving[BlockActorName] . Rotation;
			if (!Player -> IsMoving) {
				const FString AttachSide = Cast<AFoundation>(BuildItem) -> BlockActorSide;
				const float Side = Cast<AFoundation>(BuildItem) -> HalfXY * FStatic::Two;
				if (AttachSide == FStatic::Right && !Saving[BlockActorName] . Right) {
					const float CalX = FMath::Cos(FMath::DegreesToRadians(BlockActorRotation . Yaw + FStatic::Ninety)) *
						Side;
					const float CalY = FMath::Sin(FMath::DegreesToRadians(BlockActorRotation . Yaw + FStatic::Ninety)) *
						Side;
					BuildLocation = BlockActorLocation + FVector(CalX, CalY, FStatic::Zero);
					BuildRotation = BlockActorRotation . Yaw;
				}
				if (AttachSide == FStatic::Low && !Saving[BlockActorName] . Low) {
					const float CalX = FMath::Cos(
						FMath::DegreesToRadians(BlockActorRotation . Yaw + FStatic::HundredAndEighty)) * Side;
					const float CalY = FMath::Sin(
						FMath::DegreesToRadians(BlockActorRotation . Yaw + FStatic::HundredAndEighty)) * Side;
					BuildLocation = BlockActorLocation + FVector(CalX, CalY, FStatic::Zero);
					BuildRotation = BlockActorRotation . Yaw;
				}
				if (AttachSide == FStatic::Left && !Saving[BlockActorName] . Left) {
					const float CalX = FMath::Cos(
						FMath::DegreesToRadians(BlockActorRotation . Yaw + FStatic::TwoHundredAndSeventy)) * Side;
					const float CalY = FMath::Sin(
						FMath::DegreesToRadians(BlockActorRotation . Yaw + FStatic::TwoHundredAndSeventy)) * Side;
					BuildLocation = BlockActorLocation + FVector(CalX, CalY, FStatic::Zero);
					BuildRotation = BlockActorRotation . Yaw;
				}
				if (AttachSide == FStatic::Up && !Saving[BlockActorName] . Up) {
					const float CalX = FMath::Cos(
						FMath::DegreesToRadians(BlockActorRotation . Yaw + FStatic::ThreeHundredAndSixty)) * Side;
					const float CalY = FMath::Sin(
						FMath::DegreesToRadians(BlockActorRotation . Yaw + FStatic::ThreeHundredAndSixty)) * Side;
					BuildLocation = BlockActorLocation + FVector(CalX, CalY, FStatic::Zero);
					BuildRotation = BlockActorRotation . Yaw;
				}
			}
		}
		Cast<AFoundation>(BuildItem) -> SetActorRotation(FRotator(FStatic::Zero, BuildRotation, FStatic::Zero));
		Cast<AFoundation>(BuildItem) -> SetActorLocation(BuildLocation);
	}
}

bool UBuildSystem::FoundationBuild()
{
	if (BuildItem != nullptr) {
		if (Cast<AFoundation>(BuildItem) -> IsBlock) {
			//TODO UI显示被阻挡无法放置
			FLib::Echo(TEXT("被阻挡无法放置！"));
			return false;
		}
		Cast<AFoundation>(BuildItem) -> StaticMeshComponent -> SetMobility(EComponentMobility::Stationary);
		Cast<AFoundation>(BuildItem) -> SetCollision(ECollisionEnabled::QueryAndPhysics);
		Cast<AFoundation>(BuildItem) -> SetMaterial(FStatic::WoodMaterial);
		Cast<AFoundation>(BuildItem) -> PrimaryActorTick . bCanEverTick = false;
		Cast<AFoundation>(BuildItem) -> IsSet = true;
		FBuildCache Cache;
		Cache . HealthPoints = FStatic::Hundred;
		Cache . Type = FStatic::Foundation;
		Cache . Building = BuildItem;
		Cache . Location = BuildLocation;
		Cache . Rotation = Cast<AFoundation>(BuildItem) -> GetActorRotation();
		if (Cast<AFoundation>(BuildItem) -> IsAttach) {
			for (auto& Item : Cast<AFoundation>(BuildItem) -> BlockSideCache) {
				if (Item . Value . Right) {
					Saving[Item . Key] . Right = Item . Value . Right;
				}
				if (Item . Value . Low) {
					Saving[Item . Key] . Low = Item . Value . Low;
				}
				if (Item . Value . Left) {
					Saving[Item . Key] . Left = Item . Value . Left;
				}
				if (Item . Value . Up) {
					Saving[Item . Key] . Up = Item . Value . Up;
				}
			}
			Cache . Right = Cast<AFoundation>(BuildItem) -> Right;
			Cache . Low = Cast<AFoundation>(BuildItem) -> Low;
			Cache . Left = Cast<AFoundation>(BuildItem) -> Left;
			Cache . Up = Cast<AFoundation>(BuildItem) -> Up;
		}
		Saving . Emplace(Cast<AFoundation>(BuildItem) -> GetName(), Cache);
		BuildType = nullptr;
		BuildItem = nullptr;
		return true;
	}
	return false;
}


void UBuildSystem::Wall()
{
	if (BuildType != FStatic::Wall) {
		this -> UnSetBuild();
	}
	if (BuildItem == nullptr) {
		BuildItem = GetWorld() -> SpawnActor<AWall>(FVector(FStatic::Zero, FStatic::Zero, FStatic::FiveThousand),
		                                            FRotator(FStatic::Zero));
		Cast<AWall>(BuildItem) -> SetCollision(ECollisionEnabled::QueryOnly);
		BuildType = FStatic::Wall;
	} else {
		this -> UnSetBuild();
	}
}

void UBuildSystem::WallBlurAttach()
{
	if (BuildItem != nullptr) {
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
		float Z = FStatic::Zero;
		if (OriginAngle < -FStatic::HundredAndEighty && OriginAngle > -FStatic::TwoHundredAndSeventy) {
			Z = FMath::Tan(FMath::DegreesToRadians(OriginAngle + FStatic::TwoHundredAndSeventy)) * MainLocation . Z;
			if (Z > FStatic::Thousand) {
				Z = FStatic::Thousand;
			}
			X = FMath::Cos(FMath::DegreesToRadians(ViewRotation . Yaw)) * (FStatic::ThousandOne - Z);
			Y = FMath::Sin(FMath::DegreesToRadians(ViewRotation . Yaw)) * (FStatic::ThousandOne - Z);
		}
		BuildLocation = FVector(MainLocation . X, MainLocation . Y, MainLocation . Z) + FVector(X, Y, Z);
		// 检测阻挡建筑是否可以附着
		if (Cast<AWall>(BuildItem) -> IsAttach) {
			const FString BlockFoundationName = Cast<AWall>(BuildItem) -> BlockFoundationName;
			const FString BlockWallName = Cast<AWall>(BuildItem) -> BlockWallName;
			FVector BlockActorLocation;
			FRotator BlockActorRotation;
			const float Side = Cast<AWall>(BuildItem) -> HalfYZ;
			if (!BlockFoundationName . IsEmpty()) {
				BlockActorLocation = Saving[BlockFoundationName] . Location;
				BlockActorRotation = Saving[BlockFoundationName] . Rotation;
				if (!Player -> IsMoving) {
					const FString AttachSide = Cast<AWall>(BuildItem) -> BlockFoundationSide;
					if (AttachSide == FStatic::Right && !Saving[BlockFoundationName] . WallRight) {
						const float CalX = FMath::Cos(
							FMath::DegreesToRadians(BlockActorRotation . Yaw + FStatic::Ninety)) * Side;
						const float CalY = FMath::Sin(
							FMath::DegreesToRadians(BlockActorRotation . Yaw + FStatic::Ninety)) * Side;
						BuildLocation = BlockActorLocation + FVector(CalX, CalY, Side + FStatic::Thirty);
						BuildRotation = BlockActorRotation . Yaw + FStatic::Ninety;
					}
					if (AttachSide == FStatic::Low && !Saving[BlockFoundationName] . WallLow) {
						const float CalX = FMath::Cos(
							FMath::DegreesToRadians(BlockActorRotation . Yaw + FStatic::HundredAndEighty)) * Side;
						const float CalY = FMath::Sin(
							FMath::DegreesToRadians(BlockActorRotation . Yaw + FStatic::HundredAndEighty)) * Side;
						BuildLocation = BlockActorLocation + FVector(CalX, CalY, Side + FStatic::Thirty);
						BuildRotation = BlockActorRotation . Yaw + FStatic::HundredAndEighty;
					}
					if (AttachSide == FStatic::Left && !Saving[BlockFoundationName] . WallLeft) {
						const float CalX = FMath::Cos(
							FMath::DegreesToRadians(BlockActorRotation . Yaw + FStatic::TwoHundredAndSeventy)) * Side;
						const float CalY = FMath::Sin(
							FMath::DegreesToRadians(BlockActorRotation . Yaw + FStatic::TwoHundredAndSeventy)) * Side;
						BuildLocation = BlockActorLocation + FVector(CalX, CalY, Side + FStatic::Thirty);
						BuildRotation = BlockActorRotation . Yaw + FStatic::TwoHundredAndSeventy;
					}
					if (AttachSide == FStatic::Up && !Saving[BlockFoundationName] . WallUp) {
						const float CalX = FMath::Cos(
							FMath::DegreesToRadians(BlockActorRotation . Yaw + FStatic::ThreeHundredAndSixty)) * Side;
						const float CalY = FMath::Sin(
							FMath::DegreesToRadians(BlockActorRotation . Yaw + FStatic::ThreeHundredAndSixty)) * Side;
						BuildLocation = BlockActorLocation + FVector(CalX, CalY, Side + FStatic::Thirty);
						BuildRotation = BlockActorRotation . Yaw + FStatic::ThreeHundredAndSixty;
					}
				}
			}

			if (BlockFoundationName . IsEmpty() && !BlockWallName . IsEmpty()) {
				BlockActorLocation = Saving[BlockWallName] . Location;
				BlockActorRotation = Saving[BlockWallName] . Rotation;
				if (!Player -> IsMoving) {
					const FString AttachSide = Cast<AWall>(BuildItem) -> BlockWallSide;
					if (AttachSide == FStatic::Low && !Saving[BlockWallName] . Low) {
						BuildLocation = BlockActorLocation +
							FVector(FStatic::Zero, FStatic::Zero, -Side * FStatic::Two);
						BuildRotation = BlockActorRotation . Yaw;
					}
					if (AttachSide == FStatic::Up && !Saving[BlockWallName] . Up) {
						BuildLocation = BlockActorLocation +
							FVector(FStatic::Zero, FStatic::Zero, -Side * FStatic::Two);
						BuildRotation = BlockActorRotation . Yaw;
					}
				}
			}


		}
		Cast<AWall>(BuildItem) -> SetActorRotation(FRotator(FStatic::Zero, BuildRotation, FStatic::Zero));
		Cast<AWall>(BuildItem) -> SetActorLocation(BuildLocation);
	}
}

bool UBuildSystem::WallBuild()
{
	if (BuildItem != nullptr) {
		if (Cast<AWall>(BuildItem) -> IsBlock) {
			//TODO UI显示被阻挡无法放置
			FLib::Echo(TEXT("被阻挡无法放置！"));
			return false;
		}
		if (!Cast<AWall>(BuildItem) -> IsAttach) {
			//TODO UI显示被阻挡无法放置
			FLib::Echo(TEXT("未附着无法放置！"));
			return false;
		}
		Cast<AWall>(BuildItem) -> StaticMeshComponent -> SetMobility(EComponentMobility::Stationary);
		Cast<AWall>(BuildItem) -> SetCollision(ECollisionEnabled::QueryAndPhysics);
		Cast<AWall>(BuildItem) -> SetMaterial(FStatic::WoodMaterial);
		Cast<AWall>(BuildItem) -> PrimaryActorTick . bCanEverTick = false;
		Cast<AWall>(BuildItem) -> IsSet = true;
		FBuildCache Cache;
		Cache . HealthPoints = FStatic::Hundred;
		Cache . Type = FStatic::Wall;
		Cache . Building = BuildItem;
		Cache . Location = BuildLocation;
		Cache . Rotation = Cast<AWall>(BuildItem) -> GetActorRotation();
		for (auto& Item : Cast<AWall>(BuildItem) -> BlockSideCache) {
			if (Saving[Item . Key] . Type == FStatic::Foundation) {
				if (Item . Value . Right) {
					Saving[Item . Key] . WallRight = Item . Value . Right;
				}
				if (Item . Value . Low) {
					Saving[Item . Key] . WallLow = Item . Value . Low;
				}
				if (Item . Value . Left) {
					Saving[Item . Key] . WallLeft = Item . Value . Left;
				}
				if (Item . Value . Up) {
					Saving[Item . Key] . WallUp = Item . Value . Up;
				}
			} else {
				if (Item . Value . Low) {
					Saving[Item . Key] . Low = Item . Value . Low;
				} else if (Item . Value . Up) {
					Saving[Item . Key] . Up = Item . Value . Up;
				}
			}
		}
		Cache . Low = Cast<AWall>(BuildItem) -> Low;
		Cache . Up = Cast<AWall>(BuildItem) -> Up;
		Saving . Emplace(Cast<AWall>(BuildItem) -> GetName(), Cache);
		BuildType = nullptr;
		BuildItem = nullptr;
		return true;
	}
	return false;
}

void UBuildSystem::Floor()
{
	if (BuildType != FStatic::Floor) {
		this -> UnSetBuild();
	}
	if (BuildItem == nullptr) {
		BuildItem = GetWorld() -> SpawnActor<AFloor>(FVector(FStatic::Zero, FStatic::Zero, FStatic::FiveThousand),
		                                             FRotator(FStatic::Zero));
		Cast<AFloor>(BuildItem) -> SetCollision(ECollisionEnabled::QueryOnly);
		BuildType = FStatic::Floor;
	} else {
		this -> UnSetBuild();
	}
}

void UBuildSystem::FloorBlurAttach()
{
	if (BuildItem != nullptr) {
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
		float Z = FStatic::Zero;
		if (OriginAngle < -FStatic::HundredAndEighty && OriginAngle > -FStatic::TwoHundredAndSeventy) {
			Z = FMath::Tan(FMath::DegreesToRadians(OriginAngle + FStatic::TwoHundredAndSeventy)) * MainLocation . Z;
			if (Z > FStatic::Thousand) {
				Z = FStatic::Thousand;
			}
			X = FMath::Cos(FMath::DegreesToRadians(ViewRotation . Yaw)) * (FStatic::ThousandOne - Z);
			Y = FMath::Sin(FMath::DegreesToRadians(ViewRotation . Yaw)) * (FStatic::ThousandOne - Z);
		}
		BuildLocation = FVector(MainLocation . X, MainLocation . Y, MainLocation . Z) + FVector(X, Y, Z);
		// 检测阻挡建筑是否可以附着
		if (Cast<AFloor>(BuildItem) -> IsAttach && !Player -> IsLagMoving) {
			const FString BlockFoundationName = Cast<AFloor>(BuildItem) -> BlockFoundationName;
			const FString BlockWallName = Cast<AFloor>(BuildItem) -> BlockWallName;
			const FString BlockFloorName = Cast<AFloor>(BuildItem) -> BlockFloorName;
			FVector BlockActorLocation;
			FRotator BlockActorRotation;
			if ((!BlockFoundationName . IsEmpty() || !BlockFloorName . IsEmpty()) && BlockWallName . IsEmpty()) {
				FString Index;
				FString AttachSide;
				const float Side = Cast<AFloor>(BuildItem) -> HalfXY * FStatic::Two;
				if (!BlockFoundationName . IsEmpty()) {
					Index = BlockFoundationName;
					AttachSide = Cast<AFloor>(BuildItem) -> BlockFoundationSide;
				}
				if (!BlockFloorName . IsEmpty()) {
					Index = BlockFloorName;
					AttachSide = Cast<AFloor>(BuildItem) -> BlockFloorSide;
				}
				BlockActorLocation = Saving[Index] . Location;
				BlockActorRotation = Saving[Index] . Rotation;
				if (AttachSide == FStatic::Right && !Saving[Index] . Right) {
					const float CalX = FMath::Cos(FMath::DegreesToRadians(BlockActorRotation . Yaw + FStatic::Ninety)) *
						Side;
					const float CalY = FMath::Sin(FMath::DegreesToRadians(BlockActorRotation . Yaw + FStatic::Ninety)) *
						Side;
					BuildLocation = BlockActorLocation + FVector(CalX, CalY, FStatic::Twenty);
					BuildRotation = BlockActorRotation . Yaw;
				}
				if (AttachSide == FStatic::Low && !Saving[Index] . Low) {
					const float CalX = FMath::Cos(
						FMath::DegreesToRadians(BlockActorRotation . Yaw + FStatic::HundredAndEighty)) * Side;
					const float CalY = FMath::Sin(
						FMath::DegreesToRadians(BlockActorRotation . Yaw + FStatic::HundredAndEighty)) * Side;
					BuildLocation = BlockActorLocation + FVector(CalX, CalY, FStatic::Twenty);
					BuildRotation = BlockActorRotation . Yaw;
				}
				if (AttachSide == FStatic::Left && !Saving[Index] . Left) {
					const float CalX = FMath::Cos(
						FMath::DegreesToRadians(BlockActorRotation . Yaw + FStatic::TwoHundredAndSeventy)) * Side;
					const float CalY = FMath::Sin(
						FMath::DegreesToRadians(BlockActorRotation . Yaw + FStatic::TwoHundredAndSeventy)) * Side;
					BuildLocation = BlockActorLocation + FVector(CalX, CalY, FStatic::Twenty);
					BuildRotation = BlockActorRotation . Yaw;
				}
				if (AttachSide == FStatic::Up && !Saving[Index] . Up) {
					const float CalX = FMath::Cos(
						FMath::DegreesToRadians(BlockActorRotation . Yaw + FStatic::ThreeHundredAndSixty)) * Side;
					const float CalY = FMath::Sin(
						FMath::DegreesToRadians(BlockActorRotation . Yaw + FStatic::ThreeHundredAndSixty)) * Side;
					BuildLocation = BlockActorLocation + FVector(CalX, CalY, FStatic::Twenty);
					BuildRotation = BlockActorRotation . Yaw;
				}
			}

			if (!BlockWallName . IsEmpty()) {
				const float Side = Cast<AFloor>(BuildItem) -> HalfXY + Cast<AFloor>(BuildItem) -> HalfZ;
				BlockActorLocation = Saving[BlockWallName] . Location;
				BlockActorRotation = Saving[BlockWallName] . Rotation;
				const FString AttachSide = Cast<AFloor>(BuildItem) -> BlockWallSide;
				if (AttachSide == FStatic::Front && !Saving[BlockWallName] . Front) {
					const float CalX = FMath::Cos(
						FMath::DegreesToRadians(BlockActorRotation . Yaw + FStatic::HundredAndEighty)) * Side;
					const float CalY = FMath::Sin(
						FMath::DegreesToRadians(BlockActorRotation . Yaw + FStatic::HundredAndEighty)) * Side;
					BuildLocation = BlockActorLocation + FVector(CalX, CalY, FStatic::Zero);
					BuildRotation = BlockActorRotation . Yaw;
				}
				if (AttachSide == FStatic::Back && !Saving[BlockWallName] . Back) {
					const float CalX = FMath::Cos(
						FMath::DegreesToRadians(BlockActorRotation . Yaw + FStatic::ThreeHundredAndSixty)) * Side;
					const float CalY = FMath::Sin(
						FMath::DegreesToRadians(BlockActorRotation . Yaw + FStatic::ThreeHundredAndSixty)) * Side;
					BuildLocation = BlockActorLocation + FVector(CalX, CalY, FStatic::Zero);
					BuildRotation = BlockActorRotation . Yaw;
				}
			}
		}
		Cast<AFloor>(BuildItem) -> SetActorRotation(FRotator(FStatic::Zero, BuildRotation, FStatic::Zero));
		Cast<AFloor>(BuildItem) -> SetActorLocation(BuildLocation);
	}
}

bool UBuildSystem::FloorBuild()
{
	if (BuildItem != nullptr) {
		if (Cast<AFloor>(BuildItem) -> IsBlock) {
			//TODO UI显示被阻挡无法放置
			FLib::Echo(TEXT("被阻挡无法放置！"));
			return false;
		}
		if (!Cast<AFloor>(BuildItem) -> IsAttach) {
			//TODO UI显示被阻挡无法放置
			FLib::Echo(TEXT("未附着无法放置！"));
			return false;
		}
		Cast<AFloor>(BuildItem) -> StaticMeshComponent -> SetMobility(EComponentMobility::Stationary);
		Cast<AFloor>(BuildItem) -> SetCollision(ECollisionEnabled::QueryAndPhysics);
		Cast<AFloor>(BuildItem) -> SetMaterial(FStatic::WoodMaterial);
		Cast<AFloor>(BuildItem) -> PrimaryActorTick . bCanEverTick = false;
		Cast<AFloor>(BuildItem) -> IsSet = true;
		FBuildCache Cache;
		Cache . HealthPoints = FStatic::Hundred;
		Cache . Type = FStatic::Floor;
		Cache . Building = BuildItem;
		Cache . Location = BuildLocation;
		Cache . Rotation = Cast<AFloor>(BuildItem) -> GetActorRotation();
		for (auto& Item : Cast<AFloor>(BuildItem) -> BlockSideCache) {
			if (Item . Value . Right) {
				Saving[Item . Key] . Right = Item . Value . Right;
			}
			if (Item . Value . Low) {
				Saving[Item . Key] . Low = Item . Value . Low;
			}
			if (Item . Value . Left) {
				Saving[Item . Key] . Left = Item . Value . Left;
			}
			if (Item . Value . Up) {
				Saving[Item . Key] . Up = Item . Value . Up;
			}
		}
		Cache . Right = Cast<AFloor>(BuildItem) -> Right;
		Cache . Low = Cast<AFloor>(BuildItem) -> Low;
		Cache . Left = Cast<AFloor>(BuildItem) -> Left;
		Cache . Up = Cast<AFloor>(BuildItem) -> Up;
		Saving . Emplace(Cast<AFloor>(BuildItem) -> GetName(), Cache);
		BuildType = nullptr;
		BuildItem = nullptr;
		return true;
	}
	return false;
}
