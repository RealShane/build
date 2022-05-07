#pragma once

#include "CoreMinimal.h"
#include "Route.generated.h"

USTRUCT(BlueprintType)
struct FRoute
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Route;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Data;
};