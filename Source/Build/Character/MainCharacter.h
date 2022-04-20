#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Build/Lib/Static.h"
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

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//是否移动
	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	bool IsMoving = false;

	//建造模式时延迟移动解除附着
	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	bool IsLagMoving = false;

	//相机是否移动
	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	bool IsCameraMoving = false;

	//建造模式时延迟移动解除附着
	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	bool IsCameraLagMoving = false;

	//骨骼模型
	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	USkeletalMesh* SkeletalMesh;

	//摄像机摇臂
	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	USpringArmComponent* SpringArmComponent;

	//摄像机
	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	UCameraComponent* CameraComponent;

	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	UAnimSequence* AnimSequence;

	//站立待机
	FString Idle;
	//跑步
	FString Jog;
	//跳跃
	FString JumpAnim;
	FString JumpLoopAnim;

	void AnimPlay(FString Value, bool loop = false);

	FString GetPlayingAnimName() const;

	void ThirdPerson();
	void FirstPerson();

private:
	void Construct();
	void CreateModel() const;
};
