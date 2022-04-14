#pragma once

#include "CoreMinimal.h"
#include "UMG.h"
#include "Start.generated.h"

UCLASS()
class BUILD_API UStart : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	UImage* Image;

	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	UButton* Button;


	void Layout();
};
