#pragma once

#include "SlateBasics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Build/Lib/Static.h"
#include "StartHUD.h"

class BUILD_API SStartUI : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SStartUI){}
	SLATE_ARGUMENT(TWeakObjectPtr<class AStartHUD>, StartHUD)
	SLATE_ARGUMENT(TWeakObjectPtr<class ACamera>, Main)
	SLATE_END_ARGS()

	void Construct(const FArguments& args);

	TWeakObjectPtr<class AStartHUD> StartHUD;
	TWeakObjectPtr<class ACamera> Main;

	
	FReply Start();
	FReply Quit();
	void OnUserNameChanged(const FText& InText);
	void OnUserNameCommitted(const FText& InText, ETextCommit::Type CommitMethod);
	void OnPasswordChanged(const FText& InText);
	void OnPasswordCommitted(const FText& InText, ETextCommit::Type CommitMethod);
};
