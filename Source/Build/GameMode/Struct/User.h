#pragma once

#include "CoreMinimal.h"
#include "User.generated.h"

USTRUCT(BlueprintType)
struct FUser
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int UID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int CID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString KeyPre;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString NickName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Token;
};