#include "Login.h"

void ULogin::Open(URequest* Instance)
{
	Local = GetGameInstance() -> GetSubsystem<ULocal>();
	Request = Instance;
	Request -> SetLocal(Local);
	FSlateBrush LoginBrush;
	LoginBrush . SetResourceObject(LoadObject<UTexture2D>(nullptr, *FStatic::LoginImage));
	BackGroundImage -> SetBrush(LoginBrush);

	EmailInput -> SetHintText(FText::FromString(Lang . Get(FStatic::HintInputEmailText)));
	PasswordInput -> SetHintText(FText::FromString(Lang . Get(FStatic::HintInputPasswordText)));
	PasswordInput -> SetIsPassword(true);

	FSlateBrush InputBrush;
	InputBrush . SetResourceObject(LoadObject<UTexture2D>(nullptr, *FStatic::InputImage));
	FSlateFontInfo Font(FStatic::MaoBiFont, FStatic::Thirty);
	Font . LetterSpacing = FStatic::Thousand;
	FEditableTextBoxStyle InputStyle;
	InputStyle . SetFont(Font);
	InputStyle . SetForegroundColor(FColor::Black);
	InputStyle . SetFocusedForegroundColor(FColor::Black);
	InputStyle . SetBackgroundImageNormal(InputBrush);
	InputStyle . SetBackgroundColor(FColor::White);
	InputStyle . BackgroundImageNormal . DrawAs = ESlateBrushDrawType::RoundedBox;
	InputStyle . BackgroundImageHovered . DrawAs = ESlateBrushDrawType::RoundedBox;
	InputStyle . BackgroundImageFocused . DrawAs = ESlateBrushDrawType::RoundedBox;
	EmailInput -> WidgetStyle = InputStyle;
	EmailInput -> OnTextChanged . AddDynamic(this, &ThisClass::EmailChanged);
	PasswordInput -> WidgetStyle = InputStyle;
	PasswordInput -> OnTextChanged . AddDynamic(this, &ThisClass::PasswordChanged);

	FButtonStyle ButtonStyle;
	ButtonStyle . SetNormal(InputBrush);
	ButtonStyle . SetHovered(InputBrush);
	ButtonStyle . SetPressed(InputBrush);
	ButtonStyle . SetPressedForeground(FColor::Cyan);
	ButtonStyle . Normal . DrawAs = ESlateBrushDrawType::RoundedBox;
	ButtonStyle . Hovered . DrawAs = ESlateBrushDrawType::RoundedBox;
	ButtonStyle . Pressed . DrawAs = ESlateBrushDrawType::RoundedBox;
	LoginButton -> SetStyle(ButtonStyle);
	LoginButton -> OnClicked . AddDynamic(this, &ThisClass::LoginClicked);

	LoginText -> SetFont(Font);
	LoginText -> SetText(FText::FromString(Lang . Get(FStatic::LoginButtonText)));

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
}

void ULogin::Close()
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

void ULogin::EmailChanged(const FText& Email)
{
	EmailKeep = Email . ToString();
}

void ULogin::PasswordChanged(const FText& Password)
{
	PasswordKeep = Password . ToString();
}

void ULogin::LoginClicked()
{
	const FString Param = "?email=" + EmailKeep + "&password=" + PasswordKeep + "&login_type=type_ue_password";
	Request -> Login(Param);
}
