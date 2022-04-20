#pragma once

#include "GameFramework/HUD.h"
#include "MainUI.h"
#include "MainHUD.generated.h"

UCLASS()
class BUILD_API AMainHUD : public AHUD
{
	GENERATED_BODY()

public:
	virtual void PostInitializeComponents() override;

	TSharedPtr<class SMainUI> MainUI;
};
