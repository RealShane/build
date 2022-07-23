#pragma once

#include "CoreMinimal.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Build/UI/Notify/PopLayer.h"
#include "Build/Build/BuildSystem.h"
#include "Build/GameMode/Global.h"
#include "Build/GameMode/Local.h"
#include "Build/Equips/Torch.h"
#include "Build/UI/UIFacade.h"
#include "Build/Lib/NetWork.h"
#include "Build/Lib/Lang.h"
#include "MainCharacter.h"
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

	FLang Lang;

	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	UUIFacade* UI;

	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	UPopLayer* PopLayer;

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

	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	ATorch* Torch;

	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	bool EquipsLock = false;

	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	bool MenuLock = false;

	// UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	// UNetWork* NetWork;

	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	ULocal* Local;

	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	AGlobal* Global;

	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	FLatentActionInfo LatentPong;

	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	double LastReceiveTime;

	void MoveForward(float Value);
	void MoveRight(float Value);
	void LookForward(float Value);
	void LookRight(float Value);

	void PressOne();
	void PressTwo();
	void PressThree();
	void PressZero();
	void PressTab();
	void MouseLeft();
	void MouseWheelUp();
	void MouseWheelDown();
	void MouseRight();
	void Jump();
	void StopJumping();

	UFUNCTION()
	void HeartBeatPong();

	UFUNCTION(exec)
	void Print(int Bo);
	bool B = false;
	void UIRay();
	UFUNCTION(Server, Unreliable)
	void ReadBuilding(const FString& Index, const FString& HitCompName);
	UFUNCTION(Client, Unreliable)
	void PrintInClient(FBuildings Building, const FString& Index, const FString& HitCompName);
};
