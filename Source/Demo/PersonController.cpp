// Fill out your copyright notice in the Description page of Project Settings.


#include "PersonController.h"
#include "Lib.h"

APersonController::APersonController()
{
	this -> Construct();
}

void APersonController::Construct()
{
	InputComponent = CreateDefaultSubobject<UInputComponent>(TEXT("InputComponent"));
	BuildSystem = CreateDefaultSubobject<UBuildSystem>(TEXT("BuildSystem"));
}

void APersonController::BeginPlay()
{
	Super::BeginPlay();
	Main = Cast<AMainCharacter>(GetCharacter());
	BuildSystem -> SetPlayer(Main);
}

void APersonController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	this -> MoveAnimSwitch();
}

void APersonController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent -> BindAxis("MoveForward", this, &APersonController::MoveForward);
	InputComponent -> BindAxis("MoveRight", this, &APersonController::MoveRight);
	InputComponent -> BindAction("Jump", IE_Pressed, this, &APersonController::Jump);
	InputComponent -> BindAction("Jump", IE_Released, this, &APersonController::StopJumping);

	//数字1按键
	InputComponent -> BindAction("One", IE_Pressed, this, &APersonController::PressOne);
	//鼠标左键
	InputComponent -> BindAction("MouseLeft", IE_Pressed, this, &APersonController::MouseLeft);
}

void APersonController::MoveForward(float Value)
{
	ForwardValue = Value;
	if (Value != 0){
		const FRotator Rotation = GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		Main -> AddMovementInput(Direction, Value);
	}
}

void APersonController::MoveRight(float Value)
{
	RightValue = Value;
	if (Value != 0){
		const FRotator Rotation = GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		Main -> AddMovementInput(Direction, Value);
	}
}

void APersonController::Jump()
{
	bool IsFalling = Main -> GetCharacterMovement() -> IsFalling();
	//防止在空中继续按空格重复播放
	if (!IsFalling) {
		Main -> AnimPlay(Main -> JumpAnim);
	}
	Main -> Jump();
}

void APersonController::StopJumping()
{
	Main -> StopJumping();
}

void APersonController::PressOne()
{
	BuildSystem -> SetBlur();
}

void APersonController::MouseLeft()
{
	BuildSystem -> Building();
}


void APersonController::MoveAnimSwitch()
{
	bool IsFalling = Main -> GetCharacterMovement() -> IsFalling();
	bool IsCrouching = Main -> GetCharacterMovement() -> IsCrouching();
	bool IsPlaying = Main -> GetMesh() -> IsPlaying();
	bool IsJumpStart = Main -> GetPlayingAnimName() == Main -> JumpAnim;
	if (IsFalling && IsJumpCouldPlay) {
		//切换下落动画
		IsJumpCouldPlay = false;
		IsJogCouldPlay = true;
		IsIdleCouldPlay = true;
		if (IsJumpStart && !IsPlaying) {
			Main -> AnimPlay(Main -> JumpLoopAnim);
		}
	}else if (ForwardValue != 0 || RightValue != 0) {
		//切换跑步动画
		if (IsJogCouldPlay && !IsFalling) {
			IsJumpCouldPlay = true;
			IsJogCouldPlay = false;
			IsIdleCouldPlay = true;
			if (IsCrouching) {

			}else {
				Main -> AnimPlay(Main -> Jog, true);
			}
		}
	}else {
		//切换待机动画
		if (IsIdleCouldPlay && !IsFalling) {
			IsJumpCouldPlay = true;
			IsJogCouldPlay = true;
			IsIdleCouldPlay = false;
			if (IsCrouching) {

			}else {
				Main -> AnimPlay(Main -> Idle, true);
			}
		}
	}
}
