#include "StartUI.h"

#include "Camera.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SStartUI::Construct(const FArguments& args)
{
	StartHUD = args . _StartHUD;
	Main = args . _Main;
	ChildSlot
	[
		SNew(SOverlay)
		+ SOverlay::Slot()
		  . HAlign(HAlign_Center)
		  . VAlign(VAlign_Top)
		  . Padding(FStatic::Zero, FStatic::Ten, FStatic::Zero, FStatic::Zero)
		[
			SNew(STextBlock)
			. ColorAndOpacity(FLinearColor::White)
			. ShadowColorAndOpacity(FLinearColor::Black)
			. ShadowOffset(FIntPoint(-FStatic::One, FStatic::One))
			. Font(FCoreStyle::GetDefaultFontStyle("Regular", FStatic::Thirty))
			. Text(FText::FromString(TEXT("建  造")))
		]
		+ SOverlay::Slot()
		  . HAlign(HAlign_Center)
		  . VAlign(VAlign_Center)
		  . Padding(FStatic::Ten, FStatic::Zero, FStatic::Zero, FStatic::Zero)
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			[
				SNew(SButton)
                . Text(FText::FromString(TEXT("开 始")))
                . ButtonStyle(FCoreStyle::Get(), "NoBorder")
                . ForegroundColor(FSlateColor::UseForeground())
                . OnClicked(this, &SStartUI::Start)
			]
			+ SVerticalBox::Slot()
			. Padding(FStatic::Zero, FStatic::Fifty, FStatic::Zero, FStatic::Zero)
			[
				SNew(SButton)
                . Text(FText::FromString(TEXT("退 出")))
                . ButtonStyle(FCoreStyle::Get(), "NoBorder")
                . ForegroundColor(FSlateColor::UseStyle())
                . OnClicked(this, &SStartUI::Quit)
			]
		]
	];
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION

FReply SStartUI::Start()
{
	Main -> Start(StartHUD -> GetWorld());
	return FReply::Handled();
}

FReply SStartUI::Quit()
{
	Main -> Quit();
	return FReply::Handled();
}
