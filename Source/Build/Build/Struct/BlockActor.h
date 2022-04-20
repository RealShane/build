#pragma once

#include "CoreMinimal.h"
#include "BlockActor.generated.h"

USTRUCT(BlueprintType)
struct FBlockActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name;

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
};
