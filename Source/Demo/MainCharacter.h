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

	
	FString Idle = TEXT("AnimSequence'/Game/ParagonSunWukong/Characters/Heroes/Wukong/Animations/Idle.Idle'");
	FString Jog = TEXT("AnimSequence'/Game/ParagonSunWukong/Characters/Heroes/Wukong/Animations/Jog_Bwd.Jog_Bwd'");
	FString JogStop = TEXT("AnimSequence'/Game/ParagonSunWukong/Characters/Heroes/Wukong/Animations/Jog_Bwd_Stop.Jog_Bwd_Stop'");
	
	void AnimPlay(FString Value, bool loop = false);


private:
	void Construct();
	void CreateModel();
	void CreateCamera();

};
