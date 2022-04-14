#include "BuildGameModeBase.h"

ABuildGameModeBase::ABuildGameModeBase()
{
	DefaultPawnClass = ACamera::StaticClass();
	HUDClass = AStartHUD::StaticClass();
}

void ABuildGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	GEngine -> SetDynamicResolutionUserSetting(true);
}