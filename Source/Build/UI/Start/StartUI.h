#pragma once

#include "Engine.h"
#include "SlateBasics.h"
#include "StartHUD.h"

class BUILD_API SStartUI : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SStartUI){}
	SLATE_ARGUMENT(TWeakObjectPtr<class AStartHUD>, MainHUD)
	SLATE_END_ARGS()

	void Construct(const FArguments& args);

	FReply PlayGameClicked();

	FReply QuitGameClicked();

	TWeakObjectPtr<class AStartHUD> StartHUD;
};
