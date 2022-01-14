// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Build/BuildSystem.h"
#include "MainCharacter.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "PersonController.generated.h"

UCLASS()
class DEMO_API APersonController : public APlayerController
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	APersonController();
	virtual void Tick(float DeltaTime) override;

	virtual void SetupInputComponent() override;

	void Construct();

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

protected:
	void MoveForward(float Value);
	void MoveRight(float Value);
	void PressOne();
	void MouseLeft();
	void Jump();
	void StopJumping();
	void MoveAnimSwitch();
	
};
