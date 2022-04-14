#include "BuildSystem.h"

UBuildSystem::UBuildSystem()
{
	PrimaryComponentTick . bCanEverTick = true;

	Player = nullptr;
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
	if (BuildType == "Foundation") {
		return this -> FoundationBuild();
	}
	if (BuildType == "Wall") {
		return this -> WallBuild();
	}
	return false;
}

/**
 * 显示取消虚影
 */
void UBuildSystem::SetBuild(const FString Type)
{
	if (Type == "Foundation") {
		this -> Foundation();
	} else if (Type == "Wall") {
		this -> Wall();
	}
}

void UBuildSystem::UnSetBuild()
{
	if (BuildType == "Foundation") {
		GetWorld() -> DestroyActor(Cast<AFoundation>(BuildItem));
	} else if (BuildType == "Wall") {
		GetWorld() -> DestroyActor(Cast<AWall>(BuildItem));
	}
	BuildItem = nullptr;
	BuildType = nullptr;
}

/**
 * 虚影可移动附着
 */
void UBuildSystem::BlurAttach()
{
	if (BuildType == "Foundation") {
		this -> FoundationBlurAttach();
	} else if (BuildType == "Wall") {
		this -> WallBlurAttach();
	}
}

bool UBuildSystem::IsBuildMode() const
{
	return BuildItem != nullptr;
}

void UBuildSystem::Foundation()
{
	if (BuildType != "Foundation") {
		this -> UnSetBuild();
	}
	if (BuildItem == nullptr) {
		BuildItem = GetWorld() -> SpawnActor<AFoundation>(FVector(0, 0, 10000), FRotator(0));
		Cast<AFoundation>(BuildItem) -> SetCollision(ECollisionEnabled::QueryOnly);
		BuildType = "Foundation";
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
		float Angle = 90 - (360 - ViewRotation . Pitch);
		if (Angle <= 0) {
			Angle = 89;
		}
		BuildDistance = FMath::Tan(FMath::DegreesToRadians(Angle)) * MainLocation . Z;
		if (BuildDistance > 1000) {
			BuildDistance = 1000;
		}
		/**
		 * x = ρcos
		 * y = ρsin
		 */
		const float X = FMath::Cos(FMath::DegreesToRadians(ViewRotation . Yaw)) * BuildDistance;
		const float Y = FMath::Sin(FMath::DegreesToRadians(ViewRotation . Yaw)) * BuildDistance;
		BuildLocation = FVector(MainLocation . X, MainLocation . Y, MainLocation . Z - 100) + FVector(X, Y, 0);
		// 检测阻挡建筑是否可以附着
		if (Cast<AFoundation>(BuildItem) -> IsAttach) {
			const FString BlockActorName = Cast<AFoundation>(BuildItem) -> BlockActorName;
			const FVector BlockActorLocation = Saving[BlockActorName] . Location;
			const FRotator BlockActorRotation = Saving[BlockActorName] . Rotation;
			if (!Player -> IsMoving) {
				const FString AttachSide = Cast<AFoundation>(BuildItem) -> BlockActorSide;
				const float Side = Cast<AFoundation>(BuildItem) -> HalfXY * 2;
				if (AttachSide == "Right" && !Saving[BlockActorName] . Right) {
					const float CalX = FMath::Cos(FMath::DegreesToRadians(BlockActorRotation . Yaw + 90)) * Side;
					const float CalY = FMath::Sin(FMath::DegreesToRadians(BlockActorRotation . Yaw + 90)) * Side;
					BuildLocation = BlockActorLocation + FVector(CalX, CalY, 0);
					BuildRotation = BlockActorRotation . Yaw;
				}
				if (AttachSide == "Low" && !Saving[BlockActorName] . Low) {
					const float CalX = FMath::Cos(FMath::DegreesToRadians(BlockActorRotation . Yaw + 180)) * Side;
					const float CalY = FMath::Sin(FMath::DegreesToRadians(BlockActorRotation . Yaw + 180)) * Side;
					BuildLocation = BlockActorLocation + FVector(CalX, CalY, 0);
					BuildRotation = BlockActorRotation . Yaw;
				}
				if (AttachSide == "Left" && !Saving[BlockActorName] . Left) {
					const float CalX = FMath::Cos(FMath::DegreesToRadians(BlockActorRotation . Yaw + 270)) * Side;
					const float CalY = FMath::Sin(FMath::DegreesToRadians(BlockActorRotation . Yaw + 270)) * Side;
					BuildLocation = BlockActorLocation + FVector(CalX, CalY, 0);
					BuildRotation = BlockActorRotation . Yaw;
				}
				if (AttachSide == "Up" && !Saving[BlockActorName] . Up) {
					const float CalX = FMath::Cos(FMath::DegreesToRadians(BlockActorRotation . Yaw + 360)) * Side;
					const float CalY = FMath::Sin(FMath::DegreesToRadians(BlockActorRotation . Yaw + 360)) * Side;
					BuildLocation = BlockActorLocation + FVector(CalX, CalY, 0);
					BuildRotation = BlockActorRotation . Yaw;
				}
			}
		}
		Cast<AFoundation>(BuildItem) -> SetActorRotation(FRotator(0, BuildRotation, 0));
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
		Cast<AFoundation>(BuildItem) -> SetMaterial(TEXT(
			"Material'/Game/StarterContent/Materials/M_Wood_Floor_Walnut_Polished.M_Wood_Floor_Walnut_Polished'"));
		Cast<AFoundation>(BuildItem) -> PrimaryActorTick . bCanEverTick = false;
		Cast<AFoundation>(BuildItem) -> IsSet = true;
		FBuildCache Cache;
		Cache . HealthPoints = 100;
		Cache . Type = "Foundation";
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
	if (BuildType != "Wall") {
		this -> UnSetBuild();
	}
	if (BuildItem == nullptr) {
		BuildItem = GetWorld() -> SpawnActor<AWall>(FVector(0, 0, 10000), FRotator(0));
		Cast<AWall>(BuildItem) -> SetCollision(ECollisionEnabled::QueryOnly);
		BuildType = "Wall";
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
		const float OriginAngle = 90 - (360 - ViewRotation . Pitch);
		float Angle = OriginAngle;
		if (Angle <= 0) {
			Angle = 89;
		}
		BuildDistance = FMath::Tan(FMath::DegreesToRadians(Angle)) * MainLocation . Z;
		if (BuildDistance > 1000) {
			BuildDistance = 1000;
		}
		/**
		 * x = ρcos
		 * y = ρsin
		 */
		float X = FMath::Cos(FMath::DegreesToRadians(ViewRotation . Yaw)) * BuildDistance;
		float Y = FMath::Sin(FMath::DegreesToRadians(ViewRotation . Yaw)) * BuildDistance;
		float Z = 0;
		if (OriginAngle < -180 && OriginAngle > -270) {
			Z = FMath::Tan(FMath::DegreesToRadians(OriginAngle + 270)) * MainLocation . Z;
			if (Z > 1000) {
				Z = 1000;
			}
			X = FMath::Cos(FMath::DegreesToRadians(ViewRotation . Yaw)) * (1001 - Z);
			Y = FMath::Sin(FMath::DegreesToRadians(ViewRotation . Yaw)) * (1001 - Z);
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
					if (AttachSide == "Right" && !Saving[BlockFoundationName] . WallRight) {
						const float CalX = FMath::Cos(FMath::DegreesToRadians(BlockActorRotation . Yaw + 90)) * Side;
						const float CalY = FMath::Sin(FMath::DegreesToRadians(BlockActorRotation . Yaw + 90)) * Side;
						BuildLocation = BlockActorLocation + FVector(CalX, CalY, Side + 30);
						BuildRotation = BlockActorRotation . Yaw + 90;
					}
					if (AttachSide == "Low" && !Saving[BlockFoundationName] . WallLow) {
						const float CalX = FMath::Cos(FMath::DegreesToRadians(BlockActorRotation . Yaw + 180)) * Side;
						const float CalY = FMath::Sin(FMath::DegreesToRadians(BlockActorRotation . Yaw + 180)) * Side;
						BuildLocation = BlockActorLocation + FVector(CalX, CalY, Side + 30);
						BuildRotation = BlockActorRotation . Yaw + 180;
					}
					if (AttachSide == "Left" && !Saving[BlockFoundationName] . WallLeft) {
						const float CalX = FMath::Cos(FMath::DegreesToRadians(BlockActorRotation . Yaw + 270)) * Side;
						const float CalY = FMath::Sin(FMath::DegreesToRadians(BlockActorRotation . Yaw + 270)) * Side;
						BuildLocation = BlockActorLocation + FVector(CalX, CalY, Side + 30);
						BuildRotation = BlockActorRotation . Yaw + 270;
					}
					if (AttachSide == "Up" && !Saving[BlockFoundationName] . WallUp) {
						const float CalX = FMath::Cos(FMath::DegreesToRadians(BlockActorRotation . Yaw + 360)) * Side;
						const float CalY = FMath::Sin(FMath::DegreesToRadians(BlockActorRotation . Yaw + 360)) * Side;
						BuildLocation = BlockActorLocation + FVector(CalX, CalY, Side + 30);
						BuildRotation = BlockActorRotation . Yaw + 360;
					}
				}
			}

			if (BlockFoundationName . IsEmpty() && !BlockWallName . IsEmpty()) {
				BlockActorLocation = Saving[BlockWallName] . Location;
				BlockActorRotation = Saving[BlockWallName] . Rotation;
				if (!Player -> IsMoving) {
					const FString AttachSide = Cast<AWall>(BuildItem) -> BlockWallSide;
					if (AttachSide == "Low" && !Saving[BlockWallName] . Low) {
						BuildLocation = BlockActorLocation + FVector(0, 0, -Side * 2);
						BuildRotation = BlockActorRotation . Yaw;
					}
					if (AttachSide == "Up" && !Saving[BlockWallName] . Up) {
						BuildLocation = BlockActorLocation + FVector(0, 0, Side * 2);
						BuildRotation = BlockActorRotation . Yaw;
					}
				}
			}


		}
		Cast<AWall>(BuildItem) -> SetActorRotation(FRotator(0, BuildRotation, 0));
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
		Cast<AWall>(BuildItem) -> SetMaterial(TEXT(
			"Material'/Game/StarterContent/Materials/M_Wood_Floor_Walnut_Polished.M_Wood_Floor_Walnut_Polished'"));
		Cast<AWall>(BuildItem) -> PrimaryActorTick . bCanEverTick = false;
		Cast<AWall>(BuildItem) -> IsSet = true;
		FBuildCache Cache;
		Cache . HealthPoints = 100;
		Cache . Type = "Wall";
		Cache . Building = BuildItem;
		Cache . Location = BuildLocation;
		Cache . Rotation = Cast<AWall>(BuildItem) -> GetActorRotation();
		for (auto& Item : Cast<AWall>(BuildItem) -> BlockSideCache) {
			if (Saving[Item . Key] . Type == "Foundation") {
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
