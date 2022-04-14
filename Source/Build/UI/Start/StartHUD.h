#pragma once

#include "GameFramework/HUD.h"
#include "StartUI.h"
#include "StartHUD.generated.h"

UCLASS()
class BUILD_API AStartHUD : public AHUD
{
    GENERATED_BODY()

public:
    virtual void PostInitializeComponents() override;

    TSharedPtr<class SStartUI> StartUI;
};