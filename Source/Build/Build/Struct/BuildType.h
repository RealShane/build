#pragma once

#include "CoreMinimal.h"
#include "BuildType.generated.h"

USTRUCT(BlueprintType)
struct FBuildType
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Foundation = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Wall = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Floor = false;
};