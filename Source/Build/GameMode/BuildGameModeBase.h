#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Build/UI/Start/Camera.h"
#include "Build/UI/Start/StartHUD.h"
#include "BuildGameModeBase.generated.h"

UCLASS()
class BUILD_API ABuildGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	ABuildGameModeBase();
};
