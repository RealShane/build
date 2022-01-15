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
		if (IsBlock) {
			Lib::echo("被阻挡无法放置！");
		}else {
			Cast<AFloor>(BuildItem) -> StaticMeshComponent -> SetMobility(EComponentMobility::Stationary);
			Cast<AFloor>(BuildItem) -> SetCollision(ECollisionEnabled::QueryAndPhysics);
			Cast<AFloor>(BuildItem) -> SetMaterial(TEXT("Material'/Game/StarterContent/Materials/M_Wood_Floor_Walnut_Polished.M_Wood_Floor_Walnut_Polished'"));
			FBuildCache Cache;
			Cache.HealthPoints = 100;
			Cache.Type = "floor";
			Cache.Building = BuildItem;
			Cache.Location = BuildLocation;
			Saving.Emplace(Cache);
			BuildItem = nullptr;
			return true;
		}
	}else {
		if (!Saving.IsEmpty()) {
			for (FBuildCache& Item : Saving){
				Lib::echo(Item.Type + " - " + FString::FromInt(Item.Building -> GetUniqueID()) + " - ");
				Lib::echo(Item.Location.ToString());
			}
		}
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
		Cast<AFloor>(BuildItem) -> SetActorLocation(BuildLocation);
		Cast<AFloor>(BuildItem) -> SetActorRotation(FRotator(0, ViewRotation.Yaw, 0));
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