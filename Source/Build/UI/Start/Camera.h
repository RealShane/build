#pragma once

#include "CoreMinimal.h"
#include "JsonObjectConverter.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "Build/UI/Notify/PopLayer.h"
#include "Build/GameMode/Local.h"
#include "Build/Lib/Request.h"
#include "Build/UI/UIFacade.h"
#include "Build/Lib/Static.h"
#include "Build/Lib/Lib.h"
#include "Login.h"
#include "Menu.h"
#include "Camera.generated.h"

UCLASS()
class BUILD_API ACamera : public ACharacter
{
	GENERATED_BODY()

public:
	ACamera();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	ULocal* Local;

	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	URequest* Request;
	
	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	UUIFacade* UI;

	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	ULogin* Login;

	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	UMenu* Menu;
	
	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	UPopLayer* PopLayer;

	void SwitchToServerList();

	UFUNCTION()
	void Pop(FString Message, FString Color);
};
