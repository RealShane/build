#include "StartHUD.h"

void AStartHUD::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	SAssignNew(StartUI, SStartUI) . MainHUD(this);

	if (GEngine -> IsValidLowLevel()) {
		GEngine -> GameViewport -> AddViewportWidgetContent(
			SNew(SWeakWidget) . PossiblyNullContent(StartUI . ToSharedRef()));
	}
}

// void AMainHUD::PlayGameClicked()
// {
// }
//
// void AMainHUD::QuitGameClicked()
// {
// }
