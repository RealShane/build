#pragma once

#include "CoreMinimal.h"
#include "Components/Border.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Build/Lib/Static.h"
#include "PopLayer.generated.h"

UCLASS()
class BUILD_API UPopLayer : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void Pop(FString PopMessage, FString Color = FStatic::FontMaterial, int Remaining = FStatic::Five, int Ratio = FStatic::Thousand);

private:

	UPROPERTY(meta = (BindWidget))
	UBorder* BackGround;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Message;

	float Wait = FStatic::Zero;
	int Origin;
	float ShowX;
	float HideX;

	int TimeRemaining;
	int FlowRatio;

	UPROPERTY()
	UCanvasPanelSlot* BackGroundSlot;

	void SetPosition(int Index) const;

	void PushAndPull(float InDeltaTime);
	
};
