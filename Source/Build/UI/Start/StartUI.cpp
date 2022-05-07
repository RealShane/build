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
				SNew(SBox)
				. WidthOverride(FStatic::FiveHundred)
				. HeightOverride(FStatic::Fifty)
				[
					SNew(SEditableText)
					. OnTextChanged(this, &SStartUI::OnUserNameChanged)
					. OnTextCommitted(this, &SStartUI::OnUserNameCommitted)
					. Font(FCoreStyle::GetDefaultFontStyle("Regular", FStatic::Thirty))
					. HintText(FText::FromString(TEXT("请 输 入 账 号")))
				]
			]
			+ SVerticalBox::Slot()
			.Padding(FStatic::Zero, FStatic::Five, FStatic::Zero, FStatic::Zero)
			[
				SNew(SBox)
				. WidthOverride(FStatic::FiveHundred)
				. HeightOverride(FStatic::Fifty)
				[
					SNew(SEditableText)
					. OnTextChanged(this, &SStartUI::OnPasswordChanged)
					. OnTextCommitted(this, &SStartUI::OnPasswordCommitted)
					. Font(FCoreStyle::GetDefaultFontStyle("Regular", FStatic::Thirty))
					. HintText(FText::FromString(TEXT("请 输 入 密 码")))
				]
			]
		]
		+ SOverlay::Slot()
		  . HAlign(HAlign_Center)
		  . VAlign(VAlign_Center)
		  . Padding(FStatic::Ten, FStatic::Hundred, FStatic::Zero, FStatic::Zero)
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			. Padding(FStatic::Zero, FStatic::Fifty, FStatic::Zero, FStatic::Zero)
			[
				SNew(SBox)
				. WidthOverride(FStatic::TwoHundred)
				. HeightOverride(FStatic::Hundred)
				[
					SNew(SButton)
					. HAlign(HAlign_Center)
					. VAlign(VAlign_Center)
					. ButtonStyle(FCoreStyle::Get(), "NoBorder")
					. OnClicked(this, &SStartUI::Start)
					. ForegroundColor(FSlateColor::UseForeground())
					[
						SNew(SBorder)
						. BorderImage(FCoreStyle::Get() . GetBrush("ColorSpectrum.Spectrum"))
						[
							SNew(STextBlock)
							. Text(FText::FromString(TEXT("登 录")))
							. Font(FCoreStyle::GetDefaultFontStyle("Regular", FStatic::Thirty))
							. ColorAndOpacity(FLinearColor(1, 0, 1, 1))
							. HighlightColor(FLinearColor(1, 1, 0, 1))
							. ShadowColorAndOpacity(FLinearColor::Black)
							. ShadowOffset(FIntPoint(1, -1))
						]
					]
				]
			]
			+ SVerticalBox::Slot()
			. Padding(FStatic::Zero, FStatic::Twenty, FStatic::Zero, FStatic::Zero)
			[
				SNew(SBox)
				. WidthOverride(FStatic::TwoHundred)
				. HeightOverride(FStatic::Hundred)
				[
					SNew(SButton)
					. HAlign(HAlign_Center)
					. VAlign(VAlign_Center)
					. ButtonStyle(FCoreStyle::Get(), "NoBorder")
					. OnClicked(this, &SStartUI::Quit)
					. ForegroundColor(FSlateColor::UseForeground())
					[
						SNew(SBorder)
						. BorderImage(FCoreStyle::Get() . GetBrush("ColorSpectrum.Spectrum"))
						[
							SNew(STextBlock)
							. Text(FText::FromString(TEXT("退 出")))
							. Font(FCoreStyle::GetDefaultFontStyle("Regular", FStatic::Thirty))
							. ColorAndOpacity(FLinearColor(1, 0, 1, 1))
							. HighlightColor(FLinearColor(1, 1, 0, 1))
							. ShadowColorAndOpacity(FLinearColor::Black)
							. ShadowOffset(FIntPoint(1, -1))
						]
					]
				]
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

void SStartUI::OnUserNameChanged(const FText& InText)
{
	FString SText = InText.ToString();
	FLib::Echo(SText);
	FLib::Echo(SText, true);
}

void SStartUI::OnUserNameCommitted(const FText& InText, ETextCommit::Type CommitMethod)
{
	FString SText = InText.ToString();
	FLib::Echo(SText);
	FLib::Echo(SText, true);
}

void SStartUI::OnPasswordChanged(const FText& InText)
{
	FString SText = InText.ToString();
	FLib::Echo(SText);
	FLib::Echo(SText, true);
}

void SStartUI::OnPasswordCommitted(const FText& InText, ETextCommit::Type CommitMethod)
{
	FString SText = InText.ToString();
	FLib::Echo(SText);
	FLib::Echo(SText, true);
}