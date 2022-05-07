#pragma once

#include "CoreMinimal.h"
#include "BlockActor.generated.h"

USTRUCT(BlueprintType)
struct FBlockActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Right = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Low = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Left = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Up = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Front = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Back = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool DownWallRight = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool DownWallLow = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool DownWallLeft = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool DownWallUp = false;
};
