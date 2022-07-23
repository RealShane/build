#include "MainController.h"

AMainController::AMainController()
{
	InputComponent = CreateDefaultSubobject<UInputComponent>(*FStatic::InputComponent);
	BuildSystem = CreateDefaultSubobject<UBuildSystem>(*FStatic::BuildSystem);
	UI = CreateDefaultSubobject<UUIFacade>(*FStatic::UI);
	// NetWork = CreateDefaultSubobject<UNetWork>(*FStatic::NetWork);
}

void AMainController::BeginPlay()
{
	Super::BeginPlay();
	Local = GetGameInstance() -> GetSubsystem<ULocal>();
	Global = GetWorld() -> GetGameState<AGlobal>();
	Main = Cast<AMainCharacter>(GetCharacter());
	PopLayer = UI -> GetInstance<UPopLayer>(*FStatic::PopLayerBP);
	BuildSystem -> SetPlayer(Main);
	this -> bShowMouseCursor = false;
	// NetWork -> Init(GetWorld());
	LatentPong . CallbackTarget = this;
	LatentPong . ExecutionFunction = *FStatic::HeartBeatPongMethod;
	LatentPong . Linkage = FStatic::One;
	LatentPong . UUID = FStatic::TwoHundred;
}

void AMainController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UIRay();
	// FTimerHandle Handle;
	// FTimerDelegate Delegate;
	// Delegate . BindUObject(this, &ThisClass::HeartBeatPong);
	// GetWorldTimerManager() . SetTimer(Handle, Delegate, FStatic::Five, true);
	UKismetSystemLibrary::Delay(this, FStatic::Five, LatentPong);
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
	// 键盘按键
	InputComponent -> BindAction("Tab", IE_Pressed, this, &AMainController::PressTab);
	//鼠标左右中键
	InputComponent -> BindAction("MouseLeft", IE_Pressed, this, &AMainController::MouseLeft);
	InputComponent -> BindAction("MouseWheelUp", IE_Pressed, this, &AMainController::MouseWheelUp);
	InputComponent -> BindAction("MouseWheelDown", IE_Pressed, this, &AMainController::MouseWheelDown);
	InputComponent -> BindAction("MouseRight", IE_Pressed, this, &AMainController::MouseRight);
}

void AMainController::MoveForward(float Value)
{
	if (MenuLock) {
		ForwardValue = FStatic::Zero;
		return;
	}
	ForwardValue = Value;
	if (Value != FStatic::Zero) {
		const FRotator YawRotation(FStatic::Zero, GetControlRotation() . Yaw, FStatic::Zero);
		const FVector Direction = FRotationMatrix(YawRotation) . GetUnitAxis(EAxis::X);
		Main -> AddMovementInput(Direction, Value);
	}
	if (ForwardValue || RightValue) {
		Main -> Speed = FStatic::FiveHundred;
	} else {
		Main -> Speed = FStatic::Zero;
	}
}

void AMainController::MoveRight(float Value)
{
	if (MenuLock) {
		RightValue = FStatic::Zero;
		return;
	}
	RightValue = Value;
	if (Value != FStatic::Zero) {
		const FRotator YawRotation(FStatic::Zero, GetControlRotation() . Yaw, FStatic::Zero);
		const FVector Direction = FRotationMatrix(YawRotation) . GetUnitAxis(EAxis::Y);
		Main -> AddMovementInput(Direction, Value);
	}
	if (ForwardValue || RightValue) {
		Main -> Speed = FStatic::FiveHundred;
	} else {
		Main -> Speed = FStatic::Zero;
	}
}

void AMainController::LookForward(float Value)
{
	if (MenuLock) {
		return;
	}
	if (Value != FStatic::Zero) {
		Main -> AddControllerPitchInput(Value);
	}
}

void AMainController::LookRight(float Value)
{
	if (MenuLock) {
		return;
	}
	if (Value != FStatic::Zero) {
		Main -> AddControllerYawInput(Value);
	}
}

void AMainController::Jump()
{
	if (MenuLock) {
		return;
	}
	Main -> Jump();
}

void AMainController::StopJumping()
{
	if (MenuLock) {
		return;
	}
	Main -> StopJumping();
}

void AMainController::PressOne()
{
	if (MenuLock) {
		return;
	}
	BuildType = FStatic::Foundation;
	BuildSystem -> SetBuild(BuildType);
}

void AMainController::PressTwo()
{
	if (MenuLock) {
		return;
	}
	BuildType = FStatic::Wall;
	BuildSystem -> SetBuild(BuildType);
}

void AMainController::PressThree()
{
	if (MenuLock) {
		return;
	}
	BuildType = FStatic::Floor;
	BuildSystem -> SetBuild(BuildType);
}

void AMainController::PressZero()
{
	if (MenuLock) {
		return;
	}
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

void AMainController::PressTab()
{
	// AMainHUD* HUD = Cast<AMainHUD>(GetHUD());
	// if (!MenuLock) {
	// 	HUD -> OpenMenu();
	// } else {
	// 	HUD -> CloseMenu();
	// }
	// MenuLock = !MenuLock;
}

void AMainController::MouseLeft()
{
	if (BuildSystem -> IsBuildMode()) {
		BuildSystem -> Building();
		return;
	}
	if (!Main -> Punch && !Main -> GetCharacterMovement() -> IsFalling()) {
		Main -> Punch = true;
	}
}

void AMainController::MouseWheelUp()
{
	float ArmLength = Main -> SpringArmComponent -> TargetArmLength - FStatic::Fifty;
	if (ArmLength > FStatic::SixHundred) {
		ArmLength = FStatic::SixHundred;
	} else if (ArmLength <= FStatic::Zero) {
		ArmLength = FStatic::Zero;
		//转为第一人称时让身体跟着视角转动
		Main -> CameraComponent -> bUsePawnControlRotation = true;
		Main -> bUseControllerRotationYaw = true;
	} else {
		//转为第三人称时取消身体跟着视角转动
		Main -> CameraComponent -> bUsePawnControlRotation = false;
		Main -> bUseControllerRotationYaw = false;
	}
	Main -> SpringArmComponent -> TargetArmLength = ArmLength;
}

void AMainController::MouseWheelDown()
{
	float ArmLength = Main -> SpringArmComponent -> TargetArmLength + FStatic::Fifty;
	if (ArmLength > FStatic::SixHundred) {
		ArmLength = FStatic::SixHundred;
	} else if (ArmLength <= FStatic::Zero) {
		ArmLength = FStatic::Zero;
		//转为第一人称时让身体跟着视角转动
		Main -> CameraComponent -> bUsePawnControlRotation = true;
		Main -> bUseControllerRotationYaw = true;
	} else {
		//转为第三人称时取消身体跟着视角转动
		Main -> CameraComponent -> bUsePawnControlRotation = false;
		Main -> bUseControllerRotationYaw = false;
	}
	Main -> SpringArmComponent -> TargetArmLength = ArmLength;
}

void AMainController::MouseRight()
{
	if (BuildSystem -> IsBuildMode()) {
		BuildSystem -> SetBuild(BuildType);
		BuildType = nullptr;
	}
}

void AMainController::UIRay()
{
	if (!Main) {
		return;
	}
	FVector Direction = Main -> CameraComponent -> GetForwardVector();
	FVector Start = Main -> CameraComponent -> GetComponentLocation();
	// DrawDebugLine(GetWorld(), Start, Direction * FStatic::ThousandAndFiveHundred + Start, FColor::Red, false, 1, 0, 5);
	FHitResult Hit;
	FCollisionQueryParams Params;
	Params . AddIgnoredActor(this);
	Params . bTraceComplex = false;
	GetWorld() -> LineTraceSingleByObjectType(
		Hit, Start,
		Direction * FStatic::ThousandAndFiveHundred + Start,
		ECC_WorldStatic, Params
	);
	if (!Hit . IsValidBlockingHit()) {
		Main -> RayHit . Reset();
		return;
	}
	Main -> RayHit = Hit;
	if (B) {
		FString HitName = Hit . GetActor() -> GetName();
		if (FStr::IsBuildContain(HitName)) {
			FString HitCompName = Hit . GetComponent() -> GetName();
			FString Index = Hit . GetActor() -> Tags[0] . ToString();
			ReadBuilding(Index, HitCompName);
		}
	}
}

void AMainController::PrintInClient_Implementation(FBuildings Building, const FString& Index,
	const FString& HitCompName)
{
	FLib::Echo("--------------------", false);
	FLib::Echo("Name : " + Index + "-" + HitCompName, false);
	FLib::Echo("Right : " + FString::SanitizeFloat(Building . Right), false);
	FLib::Echo("Low : " + FString::SanitizeFloat(Building . Low), false);
	FLib::Echo("Left : " + FString::SanitizeFloat(Building . Left), false);
	FLib::Echo("Up : " + FString::SanitizeFloat(Building . Up), false);
	FLib::Echo("WallRight : " + FString::SanitizeFloat(Building . WallRight), false);
	FLib::Echo("WallLow : " + FString::SanitizeFloat(Building . WallLow), false);
	FLib::Echo("WallLeft : " + FString::SanitizeFloat(Building . WallLeft), false);
	FLib::Echo("WallUp : " + FString::SanitizeFloat(Building . WallUp), false);
	FLib::Echo("Front : " + FString::SanitizeFloat(Building . Front), false);
	FLib::Echo("Back : " + FString::SanitizeFloat(Building . Back), false);
	FLib::Echo("DownWallRight : " + FString::SanitizeFloat(Building . DownWallRight), false);
	FLib::Echo("DownWallLow : " + FString::SanitizeFloat(Building . DownWallLow), false);
	FLib::Echo("DownWallLeft : " + FString::SanitizeFloat(Building . DownWallLeft), false);
	FLib::Echo("DownWallUp : " + FString::SanitizeFloat(Building . DownWallUp), false);
}

void AMainController::ReadBuilding_Implementation(const FString& Index, const FString& HitCompName)
{
	if (!Global -> Buildings . Num()) {
		return;
	}
	PrintInClient(Global -> Buildings[Index], Index, HitCompName);
}

void AMainController::Print(const int Bo)
{
	if (Bo) {
		this -> B = true;
	} else {
		this -> B = false;
	}
}

void AMainController::HeartBeatPong()
{
	if (GetLocalRole() != ROLE_AutonomousProxy || GetNetMode() == NM_DedicatedServer) {
		return;
	}
	if (GetNetConnection() -> LastReceiveTime == LastReceiveTime) {
		PopLayer -> Pop(Lang . Get(FStatic::LostConnectionText), FStatic::WarningFontMaterial);
		Local -> LostConnection = true;
		const FSoftObjectPath Path(*FStatic::StartMap);
		const TSoftObjectPtr<UWorld> Map(Path);
		UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(), Map, true);
		return;
	}
	LastReceiveTime = GetNetConnection() -> LastReceiveTime;
}
