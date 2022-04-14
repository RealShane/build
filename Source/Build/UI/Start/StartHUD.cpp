#include "StartHUD.h"

void AStartHUD::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	SAssignNew(StartUI, SStartUI) . StartHUD(this);

	if (GEngine -> IsValidLowLevel()) {
		GEngine -> GameViewport -> AddViewportWidgetContent(
			SNew(SWeakWidget) . PossiblyNullContent(StartUI . ToSharedRef()));
	}
}
