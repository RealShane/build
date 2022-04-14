#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
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
	FString Idle = TEXT("AnimSequence'/Game/Mannequin/Animations/ThirdPersonIdle.ThirdPersonIdle'");
	//跑步
	FString Jog = TEXT("AnimSequence'/Game/Mannequin/Animations/ThirdPersonRun.ThirdPersonRun'");
	//跳跃
	FString JumpAnim = TEXT("AnimSequence'/Game/Mannequin/Animations/ThirdPersonJump_Start.ThirdPersonJump_Start'");
	FString JumpLoopAnim = TEXT("AnimSequence'/Game/Mannequin/Animations/ThirdPersonJump_Loop.ThirdPersonJump_Loop'");

	void AnimPlay(FString Value, bool loop = false);

	FString GetPlayingAnimName() const;

	void ThirdPerson();
	void FirstPerson();

private:
	void Construct();
	void CreateModel() const;
};
