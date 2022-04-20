#include "MainController.h"

AMainController::AMainController()
{
	MoveSteps = FStatic::Zero;
	CameraMoveSteps = FStatic::Zero;
	InputComponent = CreateDefaultSubobject<UInputComponent>(*FStatic::InputComponent);
	BuildSystem = CreateDefaultSubobject<UBuildSystem>(*FStatic::BuildSystem);
}

void AMainController::BeginPlay()
{
	Super::BeginPlay();
	Main = Cast<AMainCharacter>(GetCharacter());
	BuildSystem -> SetPlayer(Main);
	this -> bShowMouseCursor = false;
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
	InputComponent -> BindAxis("LookForward", this, &AMainController::LookForward);
	InputComponent -> BindAxis("LookRight", this, &AMainController::LookRight);
	InputComponent -> BindAction("Jump", IE_Pressed, this, &AMainController::Jump);
	InputComponent -> BindAction("Jump", IE_Released, this, &AMainController::StopJumping);

	//数字按键
	InputComponent -> BindAction("One", IE_Pressed, this, &AMainController::PressOne);
	InputComponent -> BindAction("Two", IE_Pressed, this, &AMainController::PressTwo);
	InputComponent -> BindAction("Three", IE_Pressed, this, &AMainController::PressThree);
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
	if (Value != FStatic::Zero) {
		const FRotator Rotation = GetControlRotation();
		const FRotator YawRotation(FStatic::Zero, Rotation . Yaw, FStatic::Zero);
		const FVector Direction = FRotationMatrix(YawRotation) . GetUnitAxis(EAxis::X);
		Main -> AddMovementInput(Direction, Value);
	}
	if (ForwardValue || RightValue) {
		Main -> IsMoving = true;
		MoveSteps += FStatic::One;
		if (MoveSteps >= FStatic::Thirty) {
			Main -> IsLagMoving = true;
		}
	} else {
		MoveSteps = FStatic::Zero;
		Main -> IsMoving = false;
		Main -> IsLagMoving = false;
	}
}

void AMainController::MoveRight(float Value)
{
	RightValue = Value;
	if (Value != FStatic::Zero) {
		const FRotator Rotation = GetControlRotation();
		const FRotator YawRotation(FStatic::Zero, Rotation . Yaw, FStatic::Zero);
		const FVector Direction = FRotationMatrix(YawRotation) . GetUnitAxis(EAxis::Y);
		Main -> AddMovementInput(Direction, Value);
	}
	if (ForwardValue || RightValue) {
		Main -> IsMoving = true;
		MoveSteps += FStatic::One;
		if (MoveSteps >= FStatic::Thirty) {
			Main -> IsLagMoving = true;
		}
	} else {
		MoveSteps = FStatic::Zero;
		Main -> IsMoving = false;
		Main -> IsLagMoving = false;
	}
}

void AMainController::LookForward(float Value)
{
	CameraForwardValue = Value;
	if (Value != FStatic::Zero) {
		Main -> AddControllerPitchInput(Value);
	}
	if (CameraForwardValue || CameraRightValue) {
		Main -> IsCameraMoving = true;
		CameraMoveSteps += FStatic::One;
		if (CameraMoveSteps >= FStatic::Thirty) {
			Main -> IsCameraLagMoving = true;
		}
	} else {
		CameraMoveSteps = FStatic::Zero;
		Main -> IsCameraMoving = false;
		Main -> IsCameraLagMoving = false;
	}
}

void AMainController::LookRight(float Value)
{
	CameraRightValue = Value;
	if (Value != FStatic::Zero) {
		Main -> AddControllerYawInput(Value);
	}
	if (CameraForwardValue || CameraRightValue) {
		Main -> IsCameraMoving = true;
		CameraMoveSteps += FStatic::One;
		if (CameraMoveSteps >= FStatic::Thirty) {
			Main -> IsCameraLagMoving = true;
		}
	} else {
		CameraMoveSteps = FStatic::Zero;
		Main -> IsCameraMoving = false;
		Main -> IsCameraLagMoving = false;
	}
}

void AMainController::Jump()
{
	//防止在空中继续按空格重复播放
	if (!Main -> GetCharacterMovement() -> IsFalling()) {
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
	BuildType = FStatic::Foundation;
	BuildSystem -> SetBuild(BuildType);
}

void AMainController::PressTwo()
{
	BuildType = FStatic::Wall;
	BuildSystem -> SetBuild(BuildType);
}

void AMainController::PressThree()
{
	BuildType = FStatic::Floor;
	BuildSystem -> SetBuild(BuildType);
}

void AMainController::PressZero()
{
	if (!EquipsLock) {
		Torch = GetWorld() -> SpawnActor<ATorch>(Main -> GetActorLocation(), FRotator(FStatic::Zero));
		Torch -> AttachToComponent(Main -> GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale,
		                           *FStatic::RightHandSocket);
		Torch -> LightOn();
	} else {
		GetWorld() -> DestroyActor(Torch);
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
	float ArmLength = Main -> SpringArmComponent -> TargetArmLength - FStatic::Fifty;
	if (ArmLength > FStatic::SixHundred) {
		ArmLength = FStatic::SixHundred;
	} else if (ArmLength < FStatic::Zero) {
		ArmLength = FStatic::Zero;
		IsThirdView = false;
		Main -> FirstPerson();
	} else {
		if (!IsThirdView) {
			Main -> ThirdPerson();
		}
	}
	Main -> SpringArmComponent -> TargetArmLength = ArmLength;
}

void AMainController::MouseWheelDown()
{
	float ArmLength = Main -> SpringArmComponent -> TargetArmLength + FStatic::Fifty;
	if (ArmLength > FStatic::SixHundred) {
		ArmLength = FStatic::SixHundred;
	} else if (ArmLength < FStatic::Zero) {
		ArmLength = FStatic::Zero;
		IsThirdView = false;
		Main -> FirstPerson();
	} else {
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
	const bool IsFalling = Main -> GetCharacterMovement() -> IsFalling();
	const bool IsCrouching = Main -> GetCharacterMovement() -> IsCrouching();
	const bool IsPlaying = Main -> GetMesh() -> IsPlaying();
	const bool IsJumpStart = Main -> GetPlayingAnimName() == Main -> JumpAnim;
	if (IsFalling && IsJumpCouldPlay) {
		//切换下落动画
		IsJumpCouldPlay = false;
		IsJogCouldPlay = true;
		IsIdleCouldPlay = true;
		if (IsJumpStart && !IsPlaying) {
			Main -> AnimPlay(Main -> JumpLoopAnim);
		}
	} else if (ForwardValue != FStatic::Zero || RightValue != FStatic::Zero) {
		//切换跑步动画
		if (IsJogCouldPlay && !IsFalling) {
			IsJumpCouldPlay = true;
			IsJogCouldPlay = false;
			IsIdleCouldPlay = true;
			if (IsCrouching) {

			} else {
				Main -> AnimPlay(Main -> Jog, true);
			}
		}
	} else {
		//切换待机动画
		if (IsIdleCouldPlay && !IsFalling) {
			IsJumpCouldPlay = true;
			IsJogCouldPlay = true;
			IsIdleCouldPlay = false;
			if (IsCrouching) {

			} else {
				Main -> AnimPlay(Main -> Idle, true);
			}
		}
	}
}
