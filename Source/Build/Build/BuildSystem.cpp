// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildSystem.h"


// Sets default values for this component's properties
UBuildSystem::UBuildSystem()
{
	PrimaryComponentTick.bCanEverTick = true;

	Player = nullptr;
}


// Called when the game starts
void UBuildSystem::BeginPlay()
{
	Super::BeginPlay();
	
}


// Called every frame
void UBuildSystem::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
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
void UBuildSystem::SetBuild(FString Type)
{
	if (Type == "Foundation") {
		this -> Foundation();
	}else if (Type == "Wall") {
		this -> Wall();
	}
}

void UBuildSystem::UnSetBuild()
{
	if (BuildType == "Foundation") {
		GetWorld() -> DestroyActor(Cast<AFoundation>(BuildItem));
	}else if (BuildType == "Wall") {
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
	}else if (BuildType == "Wall") {
		this -> WallBlurAttach();
	}
}

bool UBuildSystem::IsBuildMode()
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
	}else {
		this -> UnSetBuild();
	}
}

void UBuildSystem::FoundationBlurAttach()
{
	if (BuildItem != nullptr) {
		//视角旋转的Yaw值是极坐标ρ
		FRotator ViewRotation = Player -> GetController() -> GetControlRotation();
		FVector MainLocation = Player -> GetActorLocation();
		float BuildRotation = ViewRotation.Yaw;
		//ρ值有一个变化区间可以使建筑前后移动，这个变化区间值的求出应当使用人物与摄像机的夹角 z*tan
		float Angle = 90 - (360 - ViewRotation.Pitch);
		if (Angle <= 0) {
			Angle = 89;
		}
		BuildDistance = FMath::Tan(FMath::DegreesToRadians(Angle)) * MainLocation.Z;
		if (BuildDistance > 1000) {
			BuildDistance = 1000;
		}
		/**
		 * x = ρcos
		 * y = ρsin
		 */
		float X = FMath::Cos(FMath::DegreesToRadians(ViewRotation.Yaw)) * BuildDistance;
		float Y = FMath::Sin(FMath::DegreesToRadians(ViewRotation.Yaw)) * BuildDistance;
		BuildLocation = FVector(MainLocation.X, MainLocation.Y, MainLocation.Z - 100) + FVector(X, Y, 0);
		// 检测阻挡建筑是否可以附着
		if (Cast<AFoundation>(BuildItem) -> IsAttach) {
			FString BlockActorName = Cast<AFoundation>(BuildItem) -> BlockActorName;
			FVector BlockActorLocation = FVector::DownVector;
			FRotator BlockActorRotation = FRotator::ZeroRotator;
			int i = 0;
			for(; i < Saving.Num(); i++) {
				if (BlockActorName == Saving[i].Building -> GetName()) {
					BlockActorLocation = Saving[i].Location;
					BlockActorRotation = Saving[i].Rotation;
					break;
				}
			}
			if ((BlockActorLocation != FVector::DownVector) && (!Player -> IsMoving)) {
				FString AttachSide = Cast<AFoundation>(BuildItem) -> BlockActorSide;
				float Side = Cast<AFoundation>(BuildItem) -> HalfXY * 2;
				if (AttachSide == "Right" && !Saving[i].Right) {
					float CalX = FMath::Cos(FMath::DegreesToRadians(BlockActorRotation.Yaw + 90)) * Side;
					float CalY = FMath::Sin(FMath::DegreesToRadians(BlockActorRotation.Yaw + 90)) * Side;
					BuildLocation = BlockActorLocation + FVector(CalX, CalY, 0);
					BuildRotation = BlockActorRotation.Yaw;
				}
				if (AttachSide == "Low" && !Saving[i].Low) {
					float CalX = FMath::Cos(FMath::DegreesToRadians(BlockActorRotation.Yaw + 180)) * Side;
					float CalY = FMath::Sin(FMath::DegreesToRadians(BlockActorRotation.Yaw + 180)) * Side;
					BuildLocation = BlockActorLocation + FVector(CalX, CalY, 0);
					BuildRotation = BlockActorRotation.Yaw;
				}
				if (AttachSide == "Left" && !Saving[i].Left) {
					float CalX = FMath::Cos(FMath::DegreesToRadians(BlockActorRotation.Yaw + 270)) * Side;
					float CalY = FMath::Sin(FMath::DegreesToRadians(BlockActorRotation.Yaw + 270)) * Side;
					BuildLocation = BlockActorLocation + FVector(CalX, CalY, 0);
					BuildRotation = BlockActorRotation.Yaw;
				}
				if (AttachSide == "Up" && !Saving[i].Up) {
					float CalX = FMath::Cos(FMath::DegreesToRadians(BlockActorRotation.Yaw + 360)) * Side;
					float CalY = FMath::Sin(FMath::DegreesToRadians(BlockActorRotation.Yaw + 360)) * Side;
					BuildLocation = BlockActorLocation + FVector(CalX, CalY, 0);
					BuildRotation = BlockActorRotation.Yaw;
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
			Lib::echo(TEXT("被阻挡无法放置！"));
			return false;
		}
		Cast<AFoundation>(BuildItem) -> StaticMeshComponent -> SetMobility(EComponentMobility::Stationary);
		Cast<AFoundation>(BuildItem) -> SetCollision(ECollisionEnabled::QueryAndPhysics);
		Cast<AFoundation>(BuildItem) -> SetMaterial(TEXT("Material'/Game/StarterContent/Materials/M_Wood_Floor_Walnut_Polished.M_Wood_Floor_Walnut_Polished'"));
		Cast<AFoundation>(BuildItem) -> IsSet = true;
		FBuildCache Cache;
		Cache.HealthPoints = 100;
		Cache.Type = "Foundation";
		Cache.Building = BuildItem;
		Cache.Location = BuildLocation;
		Cache.Rotation = Cast<AFoundation>(BuildItem) -> GetActorRotation();
		if (Cast<AFoundation>(BuildItem) -> IsAttach) {
			for (int i = 0; i < Saving.Num(); i++) {
				for (FBlockActor Item : Cast<AFoundation>(BuildItem) -> BlockSideCache) {
					if (Saving[i].Building -> GetName() == Item.Name) {
						if (Item.Side == "Right") {
							Saving[i].Right = true;
						}else if (Item.Side == "Low") {
							Saving[i].Low = true;
						}else if (Item.Side == "Left") {
							Saving[i].Left = true;
						}else if (Item.Side == "Up") {
							Saving[i].Up = true;
						}
					}
				}
			}
			Cache.Right = Cast<AFoundation>(BuildItem) -> Right;
			Cache.Low = Cast<AFoundation>(BuildItem) -> Low;
			Cache.Left = Cast<AFoundation>(BuildItem) -> Left;
			Cache.Up = Cast<AFoundation>(BuildItem) -> Up;
		}
		Saving.Emplace(Cache);
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
	}else {
		this -> UnSetBuild();
	}
}

void UBuildSystem::WallBlurAttach()
{
	if (BuildItem != nullptr) {
		//视角旋转的Yaw值是极坐标ρ
		FRotator ViewRotation = Player -> GetController() -> GetControlRotation();
		FVector MainLocation = Player -> GetActorLocation();
		float BuildRotation = ViewRotation.Yaw;
		//ρ值有一个变化区间可以使建筑前后移动，这个变化区间值的求出应当使用人物与摄像机的夹角 z*tan
		float OriginAngle = 90 - (360 - ViewRotation.Pitch);
		float Angle = OriginAngle;
		if (Angle <= 0) {
			Angle = 89;
		}
		BuildDistance = FMath::Tan(FMath::DegreesToRadians(Angle)) * MainLocation.Z;
		if (BuildDistance > 1000) {
			BuildDistance = 1000;
		}
		/**
		 * x = ρcos
		 * y = ρsin
		 */
		float X = FMath::Cos(FMath::DegreesToRadians(ViewRotation.Yaw)) * BuildDistance;
		float Y = FMath::Sin(FMath::DegreesToRadians(ViewRotation.Yaw)) * BuildDistance;
		float Z = 0;
		if (OriginAngle < -180 && OriginAngle > -270) {
			Z = FMath::Tan(FMath::DegreesToRadians(OriginAngle + 270)) * MainLocation.Z;
			if (Z > 1000) {
				Z = 1000;
			}
			X = FMath::Cos(FMath::DegreesToRadians(ViewRotation.Yaw)) * (1001 - Z);
			Y = FMath::Sin(FMath::DegreesToRadians(ViewRotation.Yaw)) * (1001 - Z);
		}
		BuildLocation = FVector(MainLocation.X, MainLocation.Y, MainLocation.Z) + FVector(X, Y, Z);
		// 检测阻挡建筑是否可以附着
		if (Cast<AWall>(BuildItem) -> IsAttach) {
			FString BlockFoundationName = Cast<AWall>(BuildItem) -> BlockFoundationName;
			FString BlockWallName = Cast<AWall>(BuildItem) -> BlockWallName;
			FVector BlockActorLocation = FVector::DownVector;
			FRotator BlockActorRotation = FRotator::ZeroRotator;
			float Side = Cast<AWall>(BuildItem) -> HalfYZ;
			if (!BlockFoundationName.IsEmpty()) {
				int i = 0;
				for(; i < Saving.Num(); i++) {
					if (BlockFoundationName == Saving[i].Building -> GetName()) {
						BlockActorLocation = Saving[i].Location;
						BlockActorRotation = Saving[i].Rotation;
						break;
					}
				}
				if ((BlockActorLocation != FVector::DownVector) && (!Player -> IsMoving)) {
					FString AttachSide = Cast<AWall>(BuildItem) -> BlockFoundationSide;
					if (AttachSide == "Right" && !Saving[i].WallRight) {
						float CalX = FMath::Cos(FMath::DegreesToRadians(BlockActorRotation.Yaw + 90)) * Side;
						float CalY = FMath::Sin(FMath::DegreesToRadians(BlockActorRotation.Yaw + 90)) * Side;
						BuildLocation = BlockActorLocation + FVector(CalX, CalY, Side + 30);
						BuildRotation = BlockActorRotation.Yaw + 90;
					}
					if (AttachSide == "Low" && !Saving[i].WallLow) {
						float CalX = FMath::Cos(FMath::DegreesToRadians(BlockActorRotation.Yaw + 180)) * Side;
						float CalY = FMath::Sin(FMath::DegreesToRadians(BlockActorRotation.Yaw + 180)) * Side;
						BuildLocation = BlockActorLocation + FVector(CalX, CalY, Side + 30);
						BuildRotation = BlockActorRotation.Yaw + 180;
					}
					if (AttachSide == "Left" && !Saving[i].WallLeft) {
						float CalX = FMath::Cos(FMath::DegreesToRadians(BlockActorRotation.Yaw + 270)) * Side;
						float CalY = FMath::Sin(FMath::DegreesToRadians(BlockActorRotation.Yaw + 270)) * Side;
						BuildLocation = BlockActorLocation + FVector(CalX, CalY, Side + 30);
						BuildRotation = BlockActorRotation.Yaw + 270;
					}
					if (AttachSide == "Up" && !Saving[i].WallUp) {
						float CalX = FMath::Cos(FMath::DegreesToRadians(BlockActorRotation.Yaw + 360)) * Side;
						float CalY = FMath::Sin(FMath::DegreesToRadians(BlockActorRotation.Yaw + 360)) * Side;
						BuildLocation = BlockActorLocation + FVector(CalX, CalY, Side + 30);
						BuildRotation = BlockActorRotation.Yaw + 360;
					}
				}
			}
			
			if (BlockFoundationName.IsEmpty() && !BlockWallName.IsEmpty()) {
				int i = 0;
				for(; i < Saving.Num(); i++) {
					if (BlockWallName == Saving[i].Building -> GetName()) {
						BlockActorLocation = Saving[i].Location;
						BlockActorRotation = Saving[i].Rotation;
						break;
					}
				}
				if ((BlockActorLocation != FVector::DownVector) && (!Player -> IsMoving)) {
					FString AttachSide = Cast<AWall>(BuildItem) -> BlockWallSide;
					if (AttachSide == "Right" && !Saving[i].Right) {
						Lib::echo("Name : " + BlockWallName + " --- Right");
						float CalX = FMath::Cos(FMath::DegreesToRadians(BlockActorRotation.Yaw + 90)) * Side;
						float CalY = FMath::Sin(FMath::DegreesToRadians(BlockActorRotation.Yaw + 90)) * Side;
						BuildLocation = BlockActorLocation + FVector(CalX, CalY, 0);
					}
					if (AttachSide == "Low" && !Saving[i].Low) {
						Lib::echo("Name : " + BlockWallName + " --- Low");
						BuildLocation = BlockActorLocation + FVector(0, 0, -Side * 2);
					}
					if (AttachSide == "Left" && !Saving[i].Left) {
						Lib::echo("Name : " + BlockWallName + " --- Left");
						float CalX = FMath::Cos(FMath::DegreesToRadians(BlockActorRotation.Yaw + 270)) * Side;
						float CalY = FMath::Sin(FMath::DegreesToRadians(BlockActorRotation.Yaw + 270)) * Side;
						BuildLocation = BlockActorLocation + FVector(CalX, CalY, 0);
					}
					if (AttachSide == "Up" && !Saving[i].Up) {
						Lib::echo("Name : " + BlockWallName + " --- Up");
						BuildLocation = BlockActorLocation + FVector(0, 0, Side * 2);
					}
					BuildRotation = BlockActorRotation.Yaw;
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
			Lib::echo(TEXT("被阻挡无法放置！"));
			return false;
		}
		if (!Cast<AWall>(BuildItem) -> IsAttach) {
			//TODO UI显示被阻挡无法放置
			Lib::echo(TEXT("未附着无法放置！"));
			return false;
		}
		Cast<AWall>(BuildItem) -> StaticMeshComponent -> SetMobility(EComponentMobility::Stationary);
		Cast<AWall>(BuildItem) -> SetCollision(ECollisionEnabled::QueryAndPhysics);
		Cast<AWall>(BuildItem) -> SetMaterial(TEXT("Material'/Game/StarterContent/Materials/M_Wood_Floor_Walnut_Polished.M_Wood_Floor_Walnut_Polished'"));
		Cast<AWall>(BuildItem) -> IsSet = true;
		FBuildCache Cache;
		Cache.HealthPoints = 100;
		Cache.Type = "Wall";
		Cache.Building = BuildItem;
		Cache.Location = BuildLocation;
		Cache.Rotation = Cast<AWall>(BuildItem) -> GetActorRotation();
		for (int i = 0; i < Saving.Num(); i++) {
			for (FBlockActor Item : Cast<AWall>(BuildItem) -> BlockSideCache) {
				if (Saving[i].Building -> GetName() == Item.Name) {
					if (Saving[i].Type == "Foundation") {
						if (Item.Side == "Right") {
							Saving[i].WallRight = true;
						}else if (Item.Side == "Low") {
							Saving[i].WallLow = true;
						}else if (Item.Side == "Left") {
							Saving[i].WallLeft = true;
						}else if (Item.Side == "Up") {
							Saving[i].WallUp = true;
						}
					}else {
						if (Item.Side == "Right") {
							Saving[i].Right = true;
						}else if (Item.Side == "Low") {
							Saving[i].Low = true;
						}else if (Item.Side == "Left") {
							Saving[i].Left = true;
						}else if (Item.Side == "Up") {
							Saving[i].Up = true;
						}
					}
				}
			}
		}
		Cache.Right = Cast<AWall>(BuildItem) -> Right;
		Cache.Low = Cast<AWall>(BuildItem) -> Low;
		Cache.Left = Cast<AWall>(BuildItem) -> Left;
		Cache.Up = Cast<AWall>(BuildItem) -> Up;
		Saving.Emplace(Cache);
		BuildType = nullptr;
		BuildItem = nullptr;
		return true;
	}
	return false;
}