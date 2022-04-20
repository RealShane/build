#pragma once

#include "GameFramework/HUD.h"
#include "StartUI.h"
#include "StartHUD.generated.h"

UCLASS()
class BUILD_API AStartHUD : public AHUD
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	TSharedPtr<class SStartUI> StartUI;
};
