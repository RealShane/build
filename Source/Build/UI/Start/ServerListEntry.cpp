#include "ServerListEntry.h"

void UServerListEntry::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);
	const UServerListItem* Item = Cast<UServerListItem>(ListItemObject);
	Local = Item -> Local;
	KeepName = Item -> ServerName;
	KeepType = Item -> ServerType;
	KeepAddress = Item -> ServerAddress;
	ServerName -> SetText(FText::FromString(KeepType + FStatic::Space + KeepName));

	FSlateBrush InputBrush;
	InputBrush . SetResourceObject(LoadObject<UTexture2D>(nullptr, *FStatic::InputImage));
	BackGround -> Background . DrawAs = ESlateBrushDrawType::RoundedBox;
	BackGround -> SetBrush(InputBrush);

	FSlateFontInfo Font(FStatic::MaoBiFont, FStatic::Fifty);
	Font . LetterSpacing = FStatic::Thousand;
	ServerName -> SetFont(Font);
}

void UServerListEntry::NativeOnItemSelectionChanged(bool bIsSelected)
{
	IUserObjectListEntry::NativeOnItemSelectionChanged(bIsSelected);
	if (bIsSelected) {
		BackGround -> SetBrushColor(FColor::Green);
		Local -> SelectServerName = KeepName;
		Local -> SelectServerType = KeepType;
		Local -> SelectServerAddress = KeepAddress;
	} else {
		BackGround -> SetBrushColor(FColor::White);
	}
}
