#pragma once

#include "CoreMinimal.h"
#include "BuildCache.generated.h"

USTRUCT(BlueprintType)
struct FBuildCache
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HealthPoints;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Token;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UObject* Building;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Location;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator Rotation;
	
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
	
};