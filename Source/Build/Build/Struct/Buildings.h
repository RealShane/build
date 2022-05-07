#pragma once

#include "CoreMinimal.h"
#include "BuildType.h"
#include "Build/Lib/Static.h"
#include "Buildings.generated.h"

USTRUCT(BlueprintType)
struct FBuildings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HealthPoints = FStatic::Zero;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FBuildType Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UObject* Building = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Location = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator Rotation = FRotator::ZeroRotator;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Right = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Low = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Left = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Up = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool WallRight = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool WallLow = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool WallLeft = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool WallUp = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool DownWallRight = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool DownWallLow = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool DownWallLeft = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool DownWallUp = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Front = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Back = false;
};
