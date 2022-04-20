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
            . ShadowOffset(FIntPoint(-FStatic::One, FStatic::One))
            . Font(FCoreStyle::GetDefaultFontStyle("Regular", FStatic::Thirty))
            . Text(FText::FromString(TEXT("U  I")))
		]
	];
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
