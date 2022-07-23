#include "PopNotice.h"

void UPopNotice::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	Shrink(InDeltaTime);
}

void UPopNotice::Pop(const FString PopMessage, FString Color, int Size, const int Remaining, const int Ratio)
{
	ShrinkSize = Size;
	TimeRemaining = Remaining;
	Wait = FStatic::Zero;
	FlowRatio = Ratio;
	FontColor = Color;
	MessageLength = PopMessage . Len();
	
	MessageSlot = Cast<UCanvasPanelSlot>(Message -> Slot);
	MessageSlot -> SetAutoSize(true);

	UObject* FontMaterial = StaticLoadObject(UMaterial::StaticClass(), nullptr, *FontColor);
	FSlateFontInfo Font(FStatic::MaoBiFont, Size);
	if (Size > FStatic::Zero) {
		Font . LetterSpacing = FStatic::Thousand;
	}
	Font . FontMaterial = FontMaterial;
	Message -> SetFont(Font);
	Message -> SetText(FText::FromString(PopMessage));
	MessageSlot -> SetPosition(FVector2D(-Size * MessageLength, Size));

	AddToViewport();
	SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UPopNotice::SetPositionAndSize(int Data) const
{
	if (!MessageSlot) {
		return;
	}
	UObject* FontMaterial = StaticLoadObject(UMaterial::StaticClass(), nullptr, *FontColor);
	FSlateFontInfo Font(FStatic::MaoBiFont, Data);
	if (Data > FStatic::Zero) {
		Font . LetterSpacing = FStatic::Thousand;
	}
	Font . FontMaterial = FontMaterial;
	Message -> SetFont(Font);
	MessageSlot -> SetPosition(FVector2D(-Data * MessageLength, Data));
}

void UPopNotice::Shrink(float InDeltaTime)
{
	if (Wait < TimeRemaining) {
		Wait += InDeltaTime;
		return;
	}
	if (ShrinkSize > FStatic::Zero && MessageSlot) {
		ShrinkSize -= InDeltaTime * FlowRatio;
		SetPositionAndSize(ShrinkSize);
		return;
	}
	RemoveFromParent();
}