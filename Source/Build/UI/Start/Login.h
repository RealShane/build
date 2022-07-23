#pragma once

#include "CoreMinimal.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/EditableTextBox.h"
#include "Build/GameMode/Local.h"
#include "Build/Lib/Request.h"
#include "Build/Lib/Lang.h"
#include "Login.generated.h"

UCLASS()
class BUILD_API ULogin : public UUserWidget
{
	GENERATED_BODY()

public:
	FLang Lang;

	FString EmailKeep;

	FString PasswordKeep;

	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	ULocal* Local;

	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	URequest* Request;

	UPROPERTY(meta = (BindWidget))
	UImage* BackGroundImage;

	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* EmailInput;

	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* PasswordInput;

	UPROPERTY(meta = (BindWidget))
	UButton* LoginButton;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* LoginText;

	void Open(URequest* Instance);
	void Close();

	UFUNCTION()
	void EmailChanged(const FText& Email);
	UFUNCTION()
	void PasswordChanged(const FText& Password);
	UFUNCTION()
	void LoginClicked();
};
