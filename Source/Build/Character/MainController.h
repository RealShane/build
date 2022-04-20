#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "MainCharacter.h"
#include "Build/Equips/Torch.h"
#include "Build/Build/BuildSystem.h"
#include "Build/Lib/Lib.h"
#include "MainController.generated.h"

UCLASS()
class BUILD_API AMainController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	AMainController();
	virtual void Tick(float DeltaTime) override;

	virtual void SetupInputComponent() override;

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
	float CameraForwardValue;

	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	float CameraRightValue;

	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	AMainCharacter* Main;

	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	UBuildSystem* BuildSystem;

	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	FString BuildType;

	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	ATorch* Torch;

	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	bool EquipsLock = false;

	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	int MoveSteps;

	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	int CameraMoveSteps;

protected:
	void MoveForward(float Value);
	void MoveRight(float Value);
	void LookForward(float Value);
	void LookRight(float Value);

	void PressOne();
	void PressTwo();
	void PressThree();
	void PressZero();
	void MouseLeft();
	void MouseWheelUp();
	void MouseWheelDown();
	void MouseRight();
	void Jump();
	void StopJumping();
	void MoveAnimSwitch();
};
