#include "Menu.h"

#include "ServerListItem.h"

void UMenu::OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld)
{
	Close();
	Super::OnLevelRemovedFromWorld(InLevel, InWorld);
}

void UMenu::Open()
{
	Local = GetGameInstance() -> GetSubsystem<ULocal>();

	FSlateBrush LoginBrush;
	LoginBrush . SetResourceObject(LoadObject<UTexture2D>(nullptr, *FStatic::LoginImage));
	BackGroundImage -> SetBrush(LoginBrush);

	FSlateFontInfo Font(FStatic::MaoBiFont, FStatic::Fifty);
	Font . LetterSpacing = FStatic::Thousand;
	FSlateBrush InputBrush;
	InputBrush . SetResourceObject(LoadObject<UTexture2D>(nullptr, *FStatic::InputImage));
	FButtonStyle ButtonStyle;
	ButtonStyle . SetNormal(InputBrush);
	ButtonStyle . SetHovered(InputBrush);
	ButtonStyle . SetPressed(InputBrush);
	ButtonStyle . SetPressedForeground(FColor::Cyan);
	ButtonStyle . Normal . DrawAs = ESlateBrushDrawType::RoundedBox;
	ButtonStyle . Hovered . DrawAs = ESlateBrushDrawType::RoundedBox;
	ButtonStyle . Pressed . DrawAs = ESlateBrushDrawType::RoundedBox;
	JoinButton -> SetStyle(ButtonStyle);
	JoinButton -> OnClicked . AddDynamic(this, &ThisClass::JoinButtonClick);

	JoinText -> SetFont(Font);
	JoinText -> SetText(FText::FromString(Lang . Get(FStatic::JoinButtonText)));

	AddToViewport();
	SetVisibility(ESlateVisibility::Visible);
	bIsFocusable = true;
	APlayerController* PlayerController = GetWorld() -> GetFirstPlayerController();
	if (!PlayerController) {
		return;
	}
	FInputModeUIOnly UIOnly;
	UIOnly . SetWidgetToFocus(TakeWidget());
	UIOnly . SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	PlayerController -> SetInputMode(UIOnly);
	PlayerController -> SetShowMouseCursor(true);
	RenderServerList();
}

void UMenu::Close()
{
	RemoveFromParent();
	UWorld* World = GetWorld();
	if (!World) {
		return;
	}
	APlayerController* PlayerController = World -> GetFirstPlayerController();
	if (!PlayerController) {
		return;
	}
	PlayerController -> SetInputMode(FInputModeGameOnly());
	PlayerController -> SetShowMouseCursor(false);
}

void UMenu::RenderServerList()
{
	TArray<UServerListItem*> List;
	for (TMap<FString, FString> Item : Local -> Server) {
		UServerListItem* Temp = NewObject<UServerListItem>();
		Temp -> Local = Local;
		Temp -> ServerName = Item[FStatic::Name];
		Temp -> ServerType = Item[FStatic::Type];
		Temp -> ServerAddress = Item[FStatic::IP];
		List . Emplace(Temp);
	}
	ServerList -> SetListItems(List);
}

void UMenu::JoinButtonClick()
{
	if (Local -> Token . IsEmpty() || Local -> SelectServerAddress . IsEmpty()) {
		Pop . Broadcast(Lang . Get(FStatic::NeverSelectText), FStatic::WarningFontMaterial);
		return;
	}
	Pop . Broadcast(Lang . Get(FStatic::EnteringServerText), FStatic::FontMaterial);
	FSlateBrush LoadingBrush;
	LoadingBrush . SetResourceObject(LoadObject<UTexture2D>(nullptr, *FStatic::LoadingImage));
	BackGroundImage -> SetBrush(LoadingBrush);
	ServerList -> RemoveFromParent();
	JoinButton -> RemoveFromParent();
	JoinText -> RemoveFromParent();
	UGameplayStatics::OpenLevel(this, *Local -> SelectServerAddress, true);
}
