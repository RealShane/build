// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "MainCharacter.generated.h"

UCLASS()
class DEMO_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

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

	
	FString Idle = TEXT("AnimSequence'/Game/Mannequin/Animations/ThirdPersonIdle.ThirdPersonIdle'");
	FString Jog = TEXT("AnimSequence'/Game/Mannequin/Animations/ThirdPersonRun.ThirdPersonRun'");
	FString JumpAnim = TEXT("AnimSequence'/Game/Mannequin/Animations/ThirdPersonJump_Start.ThirdPersonJump_Start'");
	FString JumpLoopAnim = TEXT("AnimSequence'/Game/Mannequin/Animations/ThirdPersonJump_Loop.ThirdPersonJump_Loop'");
	FString StopJumpingAnim = TEXT("AnimSequence'/Game/Mannequin/Animations/ThirdPersonJump_End.ThirdPersonJump_End'");
	
	void AnimPlay(FString Value, bool loop = false);
	FString GetPlayingAnimName();

private:
	void Construct();
	void CreateModel();
	void CreateCamera();

};
