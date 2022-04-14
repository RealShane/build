#include "WorldGameModeBase.h"

AWorldGameModeBase::AWorldGameModeBase()
{
	PlayerControllerClass = AMainController::StaticClass();
	DefaultPawnClass = AMainCharacter::StaticClass();
}

void AWorldGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	GEngine -> SetDynamicResolutionUserSetting(true);
}
