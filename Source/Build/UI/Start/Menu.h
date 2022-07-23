#pragma once

#include "CoreMinimal.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/ListView.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Build/GameMode/Local.h"
#include "Build/Lib/Lang.h"
#include "Build/Lib/Lib.h"
#include "Menu.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FPop, FString, Message, FString, Color);

UCLASS()
class BUILD_API UMenu : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld) override;

public:
	FLang Lang;
	FPop Pop;

	UPROPERTY()
	ULocal* Local;

	UPROPERTY(meta = (BindWidget))
	UImage* BackGroundImage;

	UPROPERTY(meta = (BindWidget))
	UButton* JoinButton;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* JoinText;

	UPROPERTY(meta = (BindWidget))
	UListView* ServerList;

	UFUNCTION(BlueprintCallable)
	void Open();

	void Close();

	void RenderServerList();

	UFUNCTION()
	void JoinButtonClick();
};
