#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Build/GameMode/Local.h"
#include "ServerListItem.generated.h"

UCLASS()
class BUILD_API UServerListItem : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY()
	ULocal* Local;
	FString ServerName;
	FString ServerType;
	FString ServerAddress;
};
