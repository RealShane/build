#include "PopLayer.h"

void UPopLayer::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	PushAndPull(InDeltaTime);
}

void UPopLayer::Pop(const FString PopMessage, FString Color, const int Remaining, const int Ratio)
{
	TimeRemaining = Remaining;
	Wait = FStatic::Zero;
	FlowRatio = Ratio;
	UObject* FontMaterial = StaticLoadObject(UMaterial::StaticClass(), nullptr, *Color);
	FSlateFontInfo Font(FStatic::MaoBiFont, FStatic::Thirty);
	Font . LetterSpacing = FStatic::Thousand;
	Font . FontMaterial = FontMaterial;
	
	FSlateBrush LayerBrush;
	LayerBrush . SetResourceObject(LoadObject<UTexture2D>(nullptr, *FStatic::PopLayerImage));
	BackGround -> SetBrush(LayerBrush);
	BackGround -> SetHorizontalAlignment(HAlign_Center);
	BackGround -> SetVerticalAlignment(VAlign_Center);

	Message -> SetFont(Font);
	const int Size = PopMessage . Len() * FStatic::Seventy;
	BackGroundSlot = Cast<UCanvasPanelSlot>(BackGround -> Slot);
	BackGroundSlot -> SetAutoSize(true);

	Message -> SetText(FText::FromString(PopMessage));
	Origin = -Size;
	ShowX = -Size;
	HideX = FStatic::One;
	SetPosition(-Size);
	AddToViewport();
	SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UPopLayer::SetPosition(const int Index) const
{
	if (!BackGroundSlot) {
		return;
	}
	BackGroundSlot -> SetPosition(FVector2D(Index, BackGroundSlot -> GetSize() . Y));
}

void UPopLayer::PushAndPull(float InDeltaTime)
{
	if (ShowX < FStatic::Zero && BackGroundSlot) {
		ShowX += InDeltaTime * FlowRatio;
		if (ShowX > FStatic::Zero) {
			ShowX = FStatic::Zero;
		}
		SetPosition(ShowX);
		return;
	}
	if (Wait < TimeRemaining) {
		Wait += InDeltaTime;
		return;
	}
	if (ShowX >= FStatic::Zero && HideX > Origin && BackGroundSlot) {
		HideX -= InDeltaTime * FlowRatio;
		SetPosition(HideX);
		return;
	}
	RemoveFromParent();
}