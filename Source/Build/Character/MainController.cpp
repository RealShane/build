// Fill out your copyright notice in the Description page of Project Settings.


#include "MainController.h"

AMainController::AMainController()
{
	this -> Construct();
}

void AMainController::Construct()
{
	InputComponent = CreateDefaultSubobject<UInputComponent>(TEXT("InputComponent"));
	BuildSystem = CreateDefaultSubobject<UBuildSystem>(TEXT("BuildSystem"));
}

void AMainController::BeginPlay()
{
	Super::BeginPlay();
	Main = Cast<AMainCharacter>(GetCharacter());
	BuildSystem -> SetPlayer(Main);
}

void AMainController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	this -> MoveAnimSwitch();
}

void AMainController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent -> BindAxis("MoveForward", this, &AMainController::MoveForward);
	InputComponent -> BindAxis("MoveRight", this, &AMainController::MoveRight);
	InputComponent -> BindAction("Jump", IE_Pressed, this, &AMainController::Jump);
	InputComponent -> BindAction("Jump", IE_Released, this, &AMainController::StopJumping);

	//数字1按键
	InputComponent -> BindAction("One", IE_Pressed, this, &AMainController::PressOne);
	InputComponent -> BindAction("Two", IE_Pressed, this, &AMainController::PressTwo);
	InputComponent -> BindAction("Zero", IE_Pressed, this, &AMainController::PressZero);
	//鼠标左右中键
	InputComponent -> BindAction("MouseLeft", IE_Pressed, this, &AMainController::MouseLeft);
	InputComponent -> BindAction("MouseWheelUp", IE_Pressed, this, &AMainController::MouseWheelUp);
	InputComponent -> BindAction("MouseWheelDown", IE_Pressed, this, &AMainController::MouseWheelDown);
	InputComponent -> BindAction("MouseRight", IE_Pressed, this, &AMainController::MouseRight);
}

void AMainController::MoveForward(float Value)
{
	ForwardValue = Value;
	if (Value != 0){
		const FRotator Rotation = GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		Main -> AddMovementInput(Direction, Value);
	}
	if (ForwardValue || RightValue) {
		Main -> IsMoving = true;
	}else {
		Main -> IsMoving = false;
	}
}

void AMainController::MoveRight(float Value)
{
	RightValue = Value;
	if (Value != 0){
		const FRotator Rotation = GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		Main -> AddMovementInput(Direction, Value);
	}
	if (ForwardValue || RightValue) {
		Main -> IsMoving = true;
	}else {
		Main -> IsMoving = false;
	}
}

void AMainController::Jump()
{
	bool IsFalling = Main -> GetCharacterMovement() -> IsFalling();
	//防止在空中继续按空格重复播放
	if (!IsFalling) {
		Main -> AnimPlay(Main -> JumpAnim);
	}
	Main -> Jump();
}

void AMainController::StopJumping()
{
	Main -> StopJumping();
}

void AMainController::PressOne()
{
	BuildType = "Foundation";
	BuildSystem -> SetBuild(BuildType);
}

void AMainController::PressTwo()
{
	BuildType = "Wall";
	BuildSystem -> SetBuild(BuildType);
}

void AMainController::PressZero()
{
	if (!EquipsLock) {
		Torch = GetWorld() -> SpawnActor<ATorch>(Main -> GetActorLocation(), FRotator(0));
		FName Socket = "hand_r";
		Torch -> AttachToComponent(Main -> GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, Socket);
		Torch -> LightOn();
	}else {
		GetWorld()->DestroyActor(Torch);
	}
	EquipsLock = !EquipsLock;
}

void AMainController::MouseLeft()
{
	if (BuildSystem -> IsBuildMode()) {
		BuildSystem -> Building();
	}
}

void AMainController::MouseWheelUp()
{
	float ArmLength = Main -> SpringArmComponent -> TargetArmLength - 50;
	if (ArmLength > 600) {
		ArmLength = 600;
	}else if (ArmLength < 0) {
		ArmLength = 0;
		IsThirdView = false;
		Main -> FirstPerson();
	}else {
		if (!IsThirdView) {
			Main -> ThirdPerson();
		}
	}
	Main -> SpringArmComponent -> TargetArmLength = ArmLength;
}

void AMainController::MouseWheelDown()
{
	float ArmLength = Main -> SpringArmComponent -> TargetArmLength + 50;
	if (ArmLength > 600) {
		ArmLength = 600;
	}else if (ArmLength < 0) {
		ArmLength = 0;
		IsThirdView = false;
		Main -> FirstPerson();
	}else {
		if (!IsThirdView) {
			Main -> ThirdPerson();
		}
	}
	Main -> SpringArmComponent -> TargetArmLength = ArmLength;
}

void AMainController::MouseRight()
{
	if (BuildSystem -> IsBuildMode()) {
		BuildSystem -> SetBuild(BuildType);
	}
}


void AMainController::MoveAnimSwitch()
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
