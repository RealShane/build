#include "MainController.h"

AMainController::AMainController()
{
	InputComponent = CreateDefaultSubobject<UInputComponent>(*FStatic::InputComponent);
	BuildSystem = CreateDefaultSubobject<UBuildSystem>(*FStatic::BuildSystem);
	NetWork = CreateDefaultSubobject<UNetWork>(*FStatic::NetWork);
}

void AMainController::BeginPlay()
{
	Super::BeginPlay();
	Main = Cast<AMainCharacter>(GetCharacter());
	BuildSystem -> SetPlayer(Main);
	this -> bShowMouseCursor = false;
	NetWork -> Init(GetWorld());
	this -> Connect();
}

void AMainController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	this -> LocationSync();
	this -> MoveAnimSwitch();
	this -> UIRay();
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
	} else {
		Main -> IsMoving = false;
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
	} else {
		Main -> IsMoving = false;
	}
}

void AMainController::LookForward(float Value)
{
	if (Value != FStatic::Zero) {
		Main -> AddControllerPitchInput(Value);
	}
}

void AMainController::LookRight(float Value)
{
	if (Value != FStatic::Zero) {
		Main -> AddControllerYawInput(Value);
	}
}

void AMainController::Jump()
{
	//防止在空中继续按空格重复播放
	if (!Main -> GetCharacterMovement() -> IsFalling()) {
		Main -> AnimPlay(FStatic::JumpAnim);
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
	const bool IsJumpStart = Main -> GetPlayingAnimName() == FStatic::JumpAnim;
	if (IsFalling && IsJumpCouldPlay) {
		//切换下落动画
		IsJumpCouldPlay = false;
		IsJogCouldPlay = true;
		IsIdleCouldPlay = true;
		if (IsJumpStart && !IsPlaying) {
			Main -> AnimPlay(FStatic::JumpLoopAnim);
		}
	} else if (ForwardValue != FStatic::Zero || RightValue != FStatic::Zero) {
		//切换跑步动画
		if (IsJogCouldPlay && !IsFalling) {
			IsJumpCouldPlay = true;
			IsJogCouldPlay = false;
			IsIdleCouldPlay = true;
			if (IsCrouching) {

			} else {
				Main -> AnimPlay(FStatic::Jog, true);
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
				Main -> AnimPlay(FStatic::Idle, true);
			}
		}
	}
}

void AMainController::UIRay()
{
	FVector Direction = Main -> CameraComponent -> GetForwardVector();
	FHitResult Hit;
	FVector Start = Main -> CameraComponent -> GetComponentLocation();
	FCollisionQueryParams Params(FName(TEXT("")), false, nullptr);
	Params . AddIgnoredActor(this);
	// DrawDebugLine(GetWorld(), Start, Direction * FStatic::ThousandAndFiveHundred + Start, FColor::Red, false, 1, 0, 5);
	if (!GetWorld() -> LineTraceSingleByObjectType(Hit, Start, Direction * FStatic::ThousandAndFiveHundred + Start,
	                                               ECC_WorldStatic,
	                                               Params)) {
		Main -> RayHit . Reset();
		return;
	}
	Main -> RayHit = Hit;
	if (B) {
		FString HitName = Hit . GetActor() -> GetName();
		if (FStr::IsLand(HitName)) {
			FString HitCompName = Hit . GetComponent() -> GetName();
			FBuildings Temp = BuildSystem -> Buildings[HitName];;
			FLib::Echo("--------------------");
			FLib::Echo("Name : " + HitName + "-" + HitCompName);
			FLib::Echo("Right : " + FString::SanitizeFloat(Temp . Right));
			FLib::Echo("Low : " + FString::SanitizeFloat(Temp . Low));
			FLib::Echo("Left : " + FString::SanitizeFloat(Temp . Left));
			FLib::Echo("Up : " + FString::SanitizeFloat(Temp . Up));
			FLib::Echo("WallRight : " + FString::SanitizeFloat(Temp . WallRight));
			FLib::Echo("WallLow : " + FString::SanitizeFloat(Temp . WallLow));
			FLib::Echo("WallLeft : " + FString::SanitizeFloat(Temp . WallLeft));
			FLib::Echo("WallUp : " + FString::SanitizeFloat(Temp . WallUp));
			FLib::Echo("Front : " + FString::SanitizeFloat(Temp . Front));
			FLib::Echo("Back : " + FString::SanitizeFloat(Temp . Back));
			FLib::Echo("DownWallRight : " + FString::SanitizeFloat(Temp . DownWallRight));
			FLib::Echo("DownWallLow : " + FString::SanitizeFloat(Temp . DownWallLow));
			FLib::Echo("DownWallLeft : " + FString::SanitizeFloat(Temp . DownWallLeft));
			FLib::Echo("DownWallUp : " + FString::SanitizeFloat(Temp . DownWallUp));
		}
		if (FStr::IsContain(HitName, FStatic::Guest)) {
			FLib::Echo("--------------------");
			FLib::Echo("Guest : " + HitName);
		}
	}
}

void AMainController::Print(const int Bo)
{
	if (Bo) {
		this -> B = true;
	} else {
		this -> B = false;
	}
}

void AMainController::Connect()
{
	FString Params;
	FString Data;
	const FVector Location = Main -> GetActorLocation();
	const FRotator Rotation = Main -> GetActorRotation();
	FPlayerInfo PlayerInfo;
	PlayerInfo . Name = FDateTime::UtcNow() . ToString(*FStatic::DateTime);
	PlayerInfo . LocationX = Location . X;
	PlayerInfo . LocationY = Location . Y;
	PlayerInfo . LocationZ = Location . Z;
	PlayerInfo . RotationPitch = Rotation . Pitch;
	PlayerInfo . RotationYaw = Rotation . Yaw;
	PlayerInfo . RotationRoll = Rotation . Roll;
	FJsonObjectConverter::UStructToJsonObjectString(PlayerInfo, Data);
	FRoute Route;
	Route.Route = FStatic::FreshJoin;
	Route.Data = Data;
	FJsonObjectConverter::UStructToJsonObjectString(Route, Params);
	NetWork -> Send(Params);
}

void AMainController::LocationSync()
{
	if (!Main -> IsMoving && !Main -> GetCharacterMovement() -> IsFalling()) {
		return;
	}
	FString Params;
	FString Data;
	const FVector Location = Main -> GetActorLocation();
	const FRotator Rotation = Main -> GetActorRotation();
	FPlayerInfo PlayerInfo;
	PlayerInfo . Name = FDateTime::UtcNow() . ToString(*FStatic::DateTime);
	PlayerInfo . LocationX = Location . X;
	PlayerInfo . LocationY = Location . Y;
	PlayerInfo . LocationZ = Location . Z;
	PlayerInfo . RotationPitch = Rotation . Pitch;
	PlayerInfo . RotationYaw = Rotation . Yaw;
	PlayerInfo . RotationRoll = Rotation . Roll;
	FJsonObjectConverter::UStructToJsonObjectString(PlayerInfo, Data);
	FRoute Route;
	Route.Route = FStatic::Move;
	Route.Data = Data;
	FJsonObjectConverter::UStructToJsonObjectString(Route, Params);
	NetWork -> Send(Params);
}
