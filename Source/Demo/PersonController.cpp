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
	BuildSystem -> SetPlayer(Cast<AMainCharacter>(GetCharacter()));
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
		GetCharacter() -> AddMovementInput(Direction, Value);
	}
}

void APersonController::MoveRight(float Value)
{
	RightValue = Value;
	if (Value != 0){
		const FRotator Rotation = GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		GetCharacter() -> AddMovementInput(Direction, Value);
	}
}

void APersonController::Jump()
{
	IsJump = !IsJump;
	Cast<AMainCharacter>(GetCharacter()) -> Jump();
	Cast<AMainCharacter>(GetCharacter()) -> AnimPlay(Cast<AMainCharacter>(GetCharacter()) -> JumpAnim);
	Lib::echo(TEXT("这是Jump的IsJump ： " + FString::SanitizeFloat(IsJump)));
}

void APersonController::StopJumping()
{
	IsJump = !IsJump;
	Cast<AMainCharacter>(GetCharacter()) -> StopJumping();
	Cast<AMainCharacter>(GetCharacter()) -> AnimPlay(Cast<AMainCharacter>(GetCharacter()) -> StopJumpingAnim);
	Lib::echo(TEXT("这是StopJumping的IsJump ： " + FString::SanitizeFloat(IsJump)));
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
	if (ForwardValue != 0 || RightValue != 0) {
		if (IsJogCouldPlay) {
			IsJogCouldPlay = false;
			IsIdleCouldPlay = true;
			Cast<AMainCharacter>(GetCharacter()) -> AnimPlay(Cast<AMainCharacter>(GetCharacter()) -> Jog, true);
		}
	}else {
		if (IsIdleCouldPlay) {
			IsJogCouldPlay = true;
			IsIdleCouldPlay = false;
			Cast<AMainCharacter>(GetCharacter()) -> AnimPlay(Cast<AMainCharacter>(GetCharacter()) -> Idle, true);
		}
	}
}
