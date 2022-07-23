#pragma once

#include "CoreMinimal.h"
#include "Components/Border.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Build/Lib/Static.h"
#include "PopNotice.generated.h"

UCLASS()
class BUILD_API UPopNotice : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void Pop(FString PopMessage, FString Color = FStatic::FontMaterial, int Size = FStatic::Thirty, int Remaining = FStatic::Five, int Ratio = FStatic::Eighty);

private:

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Message;

	FString FontColor;

	float Wait = FStatic::Zero;

	int MessageLength;
	float ShrinkSize;

	int TimeRemaining;
	int FlowRatio;

	UPROPERTY()
	UCanvasPanelSlot* MessageSlot;

	void SetPositionAndSize(int Data) const;

	void Shrink(float InDeltaTime);
	
};
