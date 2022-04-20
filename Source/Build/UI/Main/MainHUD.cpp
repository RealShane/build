#include "MainHUD.h"

void AMainHUD::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	SAssignNew(MainUI, SMainUI) . MainHUD(this);

	if (GEngine -> IsValidLowLevel()) {
		GEngine -> GameViewport -> AddViewportWidgetContent(
			SNew(SWeakWidget) . PossiblyNullContent(MainUI . ToSharedRef()));
	}
}
