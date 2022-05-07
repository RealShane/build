#pragma once

#include "CoreMinimal.h"
#include "PlayerInfo.generated.h"

USTRUCT(BlueprintType)
struct FPlayerInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LocationX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LocationY;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LocationZ;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RotationPitch;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RotationYaw;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RotationRoll;
};