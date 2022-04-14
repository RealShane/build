#include "StartUI.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SStartUI::Construct(const FArguments& args)
{
	StartHUD = args . _MainHUD;

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
                . OnClicked(this, &SStartUI::PlayGameClicked)
			]
			+ SVerticalBox::Slot()
			[
				SNew(SButton)
                . Text(FText::FromString("Quit Game"))
                . OnClicked(this, &SStartUI::QuitGameClicked)
			]
		]
	];
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION

FReply SStartUI::PlayGameClicked()
{
	if (GEngine) {
		GEngine -> AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow, TEXT("PlayGameClicked"));
	}
	return FReply::Handled();
}

FReply SStartUI::QuitGameClicked()
{
	if (GEngine) {
		GEngine -> AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow, TEXT("QuitGameClicked"));
	}
	return FReply::Handled();
}
