#pragma once

#include "CoreMinimal.h"
#include "Components/Border.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "Build/GameMode/Local.h"
#include "Build/Lib/Static.h"
#include "ServerListItem.h"
#include "ServerListEntry.generated.h"

UCLASS()
class BUILD_API UServerListEntry : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

public:
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

	virtual void NativeOnItemSelectionChanged(bool bIsSelected) override;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ServerName;

	UPROPERTY(meta = (BindWidget))
	UBorder* BackGround;

	UPROPERTY()
	ULocal* Local;
	FString KeepName;
	FString KeepType;
	FString KeepAddress;
};
