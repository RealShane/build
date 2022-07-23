#pragma once

#include "CoreMinimal.h"
#include "Lib.h"

class BUILD_API FLang
{
public:

	FLang();

	inline static FString ZhCn = FPaths::ProjectContentDir() + "UI/Lang/zhCN.json";
	inline static FString EnUs = FPaths::ProjectContentDir() + "UI/Lang/enUS.json";
	
	FString Choice;

	TSharedPtr<FJsonObject> Data;

	FString Get(FString Value) const;

	void Load(FString Language);
};
