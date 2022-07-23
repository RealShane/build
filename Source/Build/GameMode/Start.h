#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Build/UI/Start/Camera.h"
#include "Start.generated.h"

UCLASS()
class BUILD_API AStart : public AGameModeBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	AStart();
};
