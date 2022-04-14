#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Build/Lib/Lib.h"
#include "Camera.generated.h"

UCLASS()
class BUILD_API ACamera : public APawn
{
	GENERATED_BODY()

public:
	ACamera();
	
protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	
	//摄像机
	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	UCameraComponent* CameraComponent;
};
