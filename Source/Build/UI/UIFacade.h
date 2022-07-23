#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UIFacade.generated.h"

UCLASS()
class BUILD_API UUIFacade : public UObject
{
	GENERATED_BODY()

public:

	
	template <typename T>
	T* GetInstance(FString Path);
	
};

template <typename T>
T* UUIFacade::GetInstance(FString Path)
{
	UClass* Widget = LoadClass<UUserWidget>(nullptr, *Path);
	return CreateWidget<T>(GetWorld(), Widget);
}