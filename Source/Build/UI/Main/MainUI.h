#pragma once

#include "Engine.h"
#include "SlateBasics.h"
#include "MainHUD.h"

class BUILD_API SMainUI : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SMainUI){}
	SLATE_ARGUMENT(TWeakObjectPtr<class AMainHUD>, MainHUD)
	SLATE_END_ARGS()

	void Construct(const FArguments& args);

	FReply PlayGameClicked();

	FReply QuitGameClicked();

	TWeakObjectPtr<class AMainHUD> MainHUD;
};
