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
	FString Side;
	
};
