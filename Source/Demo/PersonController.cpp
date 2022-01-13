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
}

void APersonController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent -> BindAxis("MoveForward", this, &APersonController::MoveForward);
	InputComponent -> BindAxis("MoveRight", this, &APersonController::MoveRight);

	//数字1按键
	InputComponent -> BindAction("One", IE_Pressed, this, &APersonController::PressOne);
	//鼠标左键
	InputComponent -> BindAction("MouseLeft", IE_Pressed, this, &APersonController::MouseLeft);
}

void APersonController::MoveForward(float Value)
{
	if (Value != 0){
		const FRotator Rotation = GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		GetCharacter() -> AddMovementInput(Direction, Value);
		if (IsMovingCouldPlay) {
			IsMovingCouldPlay = false;
			Cast<AMainCharacter>(GetCharacter()) -> AnimPlay(Cast<AMainCharacter>(GetCharacter()) -> Jog, true);
		}else {
			IsIdleCouldPlay = true;
		}
	}else {
		IsMovingCouldPlay = true;
		if (IsIdleCouldPlay) {
			Cast<AMainCharacter>(GetCharacter()) -> AnimPlay(Cast<AMainCharacter>(GetCharacter()) -> JogStop);
			IsIdleCouldPlay = false;
		}
		if (!Cast<AMainCharacter>(GetCharacter()) -> GetMesh() -> IsPlaying()) {
			Cast<AMainCharacter>(GetCharacter()) -> AnimPlay(Cast<AMainCharacter>(GetCharacter()) -> Idle);
		}
	}
}

void APersonController::MoveRight(float Value)
{
	if (Value != 0){
		const FRotator Rotation = GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		GetCharacter() -> AddMovementInput(Direction, Value);
	}
}

void APersonController::PressOne()
{
	BuildSystem -> SetBlur();
}

void APersonController::MouseLeft()
{
	BuildSystem -> Building();
}

