#include "MainUI.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SMainUI::Construct(const FArguments& args)
{
	MainHUD = args . _MainHUD;

	ChildSlot
	[
		SNew(SOverlay)
		+ SOverlay::Slot()
		  . HAlign(HAlign_Center)
		  . VAlign(VAlign_Top)
		[
			SNew(STextBlock)
            . ColorAndOpacity(FLinearColor::White)
            . ShadowColorAndOpacity(FLinearColor::Black)
            . ShadowOffset(FIntPoint(-1, 1))
            . Font(FSlateFontInfo("Arial", 26))
            . Text(FText::FromString("Main Menu"))
		]
		+ SOverlay::Slot()
		  . HAlign(HAlign_Right)
		  . VAlign(VAlign_Bottom)
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			[
				SNew(SButton)
                . Text(FText::FromString("Play Game!"))
                . OnClicked(this, &SMainUI::PlayGameClicked)
			]
			+ SVerticalBox::Slot()
			[
				SNew(SButton)
                . Text(FText::FromString("Quit Game"))
                . OnClicked(this, &SMainUI::QuitGameClicked)
			]
		]
	];
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION

FReply SMainUI::PlayGameClicked()
{
	if (GEngine) {
		GEngine -> AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow, TEXT("PlayGameClicked"));
	}
	return FReply::Handled();
}

FReply SMainUI::QuitGameClicked()
{
	if (GEngine) {
		GEngine -> AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow, TEXT("QuitGameClicked"));
	}
	return FReply::Handled();
}
