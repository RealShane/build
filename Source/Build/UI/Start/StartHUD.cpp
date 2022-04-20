#include "StartHUD.h"

void AStartHUD::BeginPlay()
{
	StartUI = SNew(SStartUI) . StartHUD(this);
	GEngine -> GameViewport -> AddViewportWidgetContent(
		SNew(SWeakWidget) . PossiblyNullContent(StartUI . ToSharedRef()));
	StartUI -> SetVisibility(EVisibility::Visible);
}
