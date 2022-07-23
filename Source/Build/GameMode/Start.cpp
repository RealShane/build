#include "Start.h"

AStart::AStart()
{
	DefaultPawnClass = ACamera::StaticClass();
}

void AStart::BeginPlay()
{
	Super::BeginPlay();
	GEngine -> SetDynamicResolutionUserSetting(true);
}