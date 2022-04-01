// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Build/Build/BuildSystem.h"
#include "MainCharacter.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "Build/Lib/Lib.h"
#include "MainController.generated.h"

UCLASS()
class BUILD_API AMainController : public APlayerController
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	AMainController();
	virtual void Tick(float DeltaTime) override;

	virtual void SetupInputComponent() override;

	void Construct();

	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	bool IsThirdView = true;

	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	bool IsJumpCouldPlay = true;
	
	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	bool IsJogCouldPlay = true;

	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	bool IsIdleCouldPlay = true;

	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	float ForwardValue;

	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	float RightValue;

	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	AMainCharacter* Main;

	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	UBuildSystem* BuildSystem;

	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	FString BuildType;

protected:
	void MoveForward(float Value);
	void MoveRight(float Value);
	void PressOne();
	void PressTwo();
	void MouseLeft();
	void MouseWheelUp();
	void MouseWheelDown();
	void MouseRight();
	void Jump();
	void StopJumping();
	void MoveAnimSwitch();
	
};
