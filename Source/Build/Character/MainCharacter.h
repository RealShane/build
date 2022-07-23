#pragma once

#include "CoreMinimal.h"
#include "Json/Public/Json.h"
#include "JsonObjectConverter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Json/Public/Serialization/JsonReader.h"
#include "Json/Public/Serialization/JsonSerializer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Build/GameMode/Struct/User.h"
#include "Build/UI/Notify/PopNotice.h"
#include "Build/UI/Notify/PopLayer.h"
#include "Build/Build/Foundation.h"
#include "Build/GameMode/Global.h"
#include "Build/GameMode/Local.h"
#include "Build/UI/Main/Main.h"
#include "Build/UI/UIFacade.h"
#include "Build/Build/Floor.h"
#include "Build/Build/Wall.h"
#include "Build/Lib/Static.h"
#include "Build/Lib/Lang.h"
#include "Build/Lib/Lib.h"
#include "MainCharacter.generated.h"

UCLASS()
class BUILD_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AMainCharacter();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	FLang Lang;

	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	float Health;

	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	UUIFacade* UI;

	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	UMain* MainUI;

	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	UPopLayer* PopLayer;
	
	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	UPopNotice* PopNotice;

	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	ULocal* Local;

	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	AGlobal* Global;

	//射线击中物
	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	FHitResult RayHit;

	//骨骼模型
	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	USkeletalMesh* SkeletalMesh;

	//摄像机摇臂
	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	USpringArmComponent* SpringArmComponent;

	//摄像机
	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	UCameraComponent* CameraComponent;

	// 蓝图动画
	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	float Speed;

	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	bool Punch;

	void Join();

	UFUNCTION(Server, Reliable)
	void AddPlayerToServer(const FString& Key, FUser Value);

	UFUNCTION(NetMulticast, Reliable)
	void JoinBroadcast(const FString& Token, const FString& NickName);
};
