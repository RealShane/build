// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildSystem.h"

#include "DrawDebugHelpers.h"
#include "Demo/Lib.h"


// Sets default values for this component's properties
UBuildSystem::UBuildSystem()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// ...
	Player = nullptr;
}


// Called when the game starts
void UBuildSystem::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
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
	if (BuildItem != nullptr) {
		bool IsBlock = Cast<AFloor>(BuildItem) -> IsBlock;
		if (IsBlock && !ForceBuild) {
			//TODO UI显示被阻挡无法放置
			Lib::echo(TEXT("被阻挡无法放置！"));
			return false;
		}
		Cast<AFloor>(BuildItem) -> StaticMeshComponent -> SetMobility(EComponentMobility::Stationary);
		Cast<AFloor>(BuildItem) -> SetCollision(ECollisionEnabled::QueryAndPhysics);
		Cast<AFloor>(BuildItem) -> SetMaterial(TEXT("Material'/Game/StarterContent/Materials/M_Wood_Floor_Walnut_Polished.M_Wood_Floor_Walnut_Polished'"));
		if (ForceBuild) {
			if (WhichSide == "Right") {
				Saving[Index].Right = true;
			}else if (WhichSide == "Low") {
				Saving[Index].Low = true;
			}else if (WhichSide == "Left") {
				Saving[Index].Left = true;
			}else if (WhichSide == "Up") {
				Saving[Index].Up = true;
			}
			Lib::echo("Right is : " + FString::SanitizeFloat(Saving[Index].Right));
			Lib::echo("Low is : " + FString::SanitizeFloat(Saving[Index].Low));
			Lib::echo("Left is : " + FString::SanitizeFloat(Saving[Index].Left));
			Lib::echo("Up is : " + FString::SanitizeFloat(Saving[Index].Up));
		}
		FBuildCache Cache;
		Cache.HealthPoints = 100;
		Cache.Type = "Floor";
		Cache.Building = BuildItem;
		Cache.Location = BuildLocation;
		Cache.Rotation = Cast<AFloor>(BuildItem) -> GetActorRotation();
		Saving.Emplace(Cache);
		BuildItem = nullptr;
		return true;
	}
	return false;
}

/**
 * 显示虚影
 */
void UBuildSystem::SetBuild()
{
	if (BuildItem == nullptr) {
		BuildItem = GetWorld() -> SpawnActor<AFloor>(FVector(0, 0, -10000), FRotator(0));
		Cast<AFloor>(BuildItem) -> SetCollision(ECollisionEnabled::QueryOnly);
	}
}

/**
 * 取消虚影
 */
void UBuildSystem::UnSetBuild()
{
	if (BuildItem != nullptr) {
		GetWorld() -> DestroyActor(Cast<AFloor>(BuildItem));
		BuildItem = nullptr;
	}
}

/**
 * 虚影可移动附着
 */
void UBuildSystem::BlurAttach()
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
		FString BlockActorName = Cast<AFloor>(BuildItem) -> BlockActorName;
		if (!BlockActorName.IsEmpty()) {
			if (!Saving.IsEmpty()) {
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
				Index = i;
				if ((BlockActorLocation != FVector::DownVector) && (!Player -> IsMoving)) {
					float Side = Cast<AFloor>(BuildItem) -> HalfXY * 2;
					float CenterToTop = Cast<AFloor>(BuildItem) -> HalfXY * FMath::Sqrt(2.f);
					//右上角
					float RightUpX = FMath::Cos(FMath::DegreesToRadians(BlockActorRotation.Yaw + 45)) * CenterToTop;
					float RightUpY = FMath::Sin(FMath::DegreesToRadians(BlockActorRotation.Yaw + 45)) * CenterToTop;
					//左上角
					float LeftUpX = FMath::Cos(FMath::DegreesToRadians(BlockActorRotation.Yaw + 135)) * CenterToTop;
					float LeftUpY = FMath::Sin(FMath::DegreesToRadians(BlockActorRotation.Yaw + 135)) * CenterToTop;
					//左下角
					float LeftLowX = FMath::Cos(FMath::DegreesToRadians(BlockActorRotation.Yaw + 225)) * CenterToTop;
					float LeftLowY = FMath::Sin(FMath::DegreesToRadians(BlockActorRotation.Yaw + 225)) * CenterToTop;
					//右下角
					float RightLowX = FMath::Cos(FMath::DegreesToRadians(BlockActorRotation.Yaw + 315)) * CenterToTop;
					float RightLowY = FMath::Sin(FMath::DegreesToRadians(BlockActorRotation.Yaw + 315)) * CenterToTop;

					FVector RightSideCenter = BlockActorLocation + FVector((RightUpX - RightLowX) / 2, (RightUpY - RightLowY) / 2, 0);
					FVector LowSideCenter = BlockActorLocation - FVector((RightLowX - LeftLowX) / 2, (RightLowY - LeftLowY) / 2, 0);
					FVector LeftSideCenter = BlockActorLocation - FVector((LeftUpX - LeftLowX) / 2, (LeftUpY - LeftLowY) / 2, 0);
					FVector UpSideCenter = BlockActorLocation + FVector((RightUpX - LeftUpX) / 2, (RightUpY - LeftUpY) / 2, 0);
					float BuildToRight = FMath::Sqrt(FMath::Square(BuildLocation.X - RightSideCenter.X) + FMath::Square(BuildLocation.Y - RightSideCenter.Y));
					float BuildToLow = FMath::Sqrt(FMath::Square(BuildLocation.X - LowSideCenter.X) + FMath::Square(BuildLocation.Y - LowSideCenter.Y));
					float BuildToLeft = FMath::Sqrt(FMath::Square(BuildLocation.X - LeftSideCenter.X) + FMath::Square(BuildLocation.Y - LeftSideCenter.Y));
					float BuildToUp = FMath::Sqrt(FMath::Square(BuildLocation.X - UpSideCenter.X) + FMath::Square(BuildLocation.Y - UpSideCenter.Y));
					float Min = FMath::Min(FMath::Min(BuildToRight, BuildToLow), FMath::Min(BuildToLeft, BuildToUp));
					ForceBuild = true;
					if (Min == BuildToRight && !Saving[i].Right) {
						float CalX = FMath::Cos(FMath::DegreesToRadians(BlockActorRotation.Yaw)) * Side;
						float CalY = FMath::Sin(FMath::DegreesToRadians(BlockActorRotation.Yaw)) * Side;
						BuildLocation = BlockActorLocation + FVector(CalX, CalY, 0);
						WhichSide = "Right";
					}else if (Min == BuildToLow && !Saving[i].Low) {
						float CalX = FMath::Cos(FMath::DegreesToRadians(BlockActorRotation.Yaw + 90)) * Side;
						float CalY = FMath::Sin(FMath::DegreesToRadians(BlockActorRotation.Yaw + 90)) * Side;
						BuildLocation = BlockActorLocation + FVector(CalX, CalY, 0);
						WhichSide = "Low";
					}else if (Min == BuildToLeft && !Saving[i].Left) {
						float CalX = FMath::Cos(FMath::DegreesToRadians(BlockActorRotation.Yaw + 180)) * Side;
						float CalY = FMath::Sin(FMath::DegreesToRadians(BlockActorRotation.Yaw + 180)) * Side;
						BuildLocation = BlockActorLocation + FVector(CalX, CalY, 0);
						WhichSide = "Left";
					}else if (Min == BuildToUp && !Saving[i].Up) {
						float CalX = FMath::Cos(FMath::DegreesToRadians(BlockActorRotation.Yaw + 270)) * Side;
						float CalY = FMath::Sin(FMath::DegreesToRadians(BlockActorRotation.Yaw + 270)) * Side;
						BuildLocation = BlockActorLocation + FVector(CalX, CalY, 0);
						WhichSide = "Up";
					}else {
						ForceBuild = false;
					}
					BuildRotation = BlockActorRotation.Yaw;
				}
			}
		}
		Cast<AFloor>(BuildItem) -> SetActorLocation(BuildLocation);
		Cast<AFloor>(BuildItem) -> SetActorRotation(FRotator(0, BuildRotation, 0));
	}
}

bool UBuildSystem::IsCollision()
{
	FHitResult OutHit;
	FVector Start = Cast<AFloor>(BuildItem) -> GetActorLocation();
	Start.X += 100;
	Start.Y += 50;
	FVector ForwardVector = Cast<AFloor>(BuildItem) -> GetActorForwardVector();
	FVector End = ((ForwardVector * 500.f) + Start);
	// FTransform MyTransform = FTransform(FVector(-500, 50, 400));
	// FBox MyBox = FBox(FVector(0, 0, 0), FVector(400, 400, 20));
	// DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 1, 0, 5);
	// DrawDebugSolidBox(GetWorld(), MyBox, FColor::White, MyTransform, true);
	if (GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_WorldStatic)){
		//获得碰撞到的物体的组件名称并打印
		Lib::echo("The Component Being Hit by Channel is : " + OutHit.GetComponent()->GetName());
		return true;
	}
	return false;
}