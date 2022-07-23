#include "MainCharacter.h"

AMainCharacter::AMainCharacter()
{
	PrimaryActorTick . bCanEverTick = true;

	Health = FStatic::Hundred;

	SkeletalMesh = LoadObject<USkeletalMesh>(nullptr, *FStatic::MainSKM);
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(*FStatic::SpringArmComponent);
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(*FStatic::CameraComponent);
	UI = CreateDefaultSubobject<UUIFacade>(*FStatic::UI);
	GetCharacterMovement() -> bOrientRotationToMovement = true;
	GetCharacterMovement() -> JumpZVelocity = FStatic::ThousandAndTwoHundred;
	GetCharacterMovement() -> AirControl = FStatic::One;
	GetCharacterMovement() -> GravityScale = FStatic::Twenty;

	GetMesh() -> SetRelativeRotation(FRotator(FStatic::Zero, -FStatic::Ninety, FStatic::Zero));
	GetMesh() -> SetRelativeLocation(FVector(FStatic::Zero, FStatic::Zero, FStatic::Ten));
	GetMesh() -> SetSkeletalMesh(SkeletalMesh);
	GetCapsuleComponent() -> InitCapsuleSize(FStatic::FortyTwo, FStatic::Hundred);

	// 摄像机和摇臂
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;// 第三人称时改为false
	bUseControllerRotationRoll = false;
	GetCharacterMovement() -> RotationRate = FRotator(FStatic::Zero, FStatic::FiveHundredAndForty, FStatic::Zero);
	SpringArmComponent -> SetupAttachment(RootComponent);
	SpringArmComponent -> SetRelativeLocation(FVector(FStatic::Ten, FStatic::Zero, FStatic::Seventy));
	SpringArmComponent -> SetRelativeRotation(FRotator(-FStatic::Fifteen, FStatic::Zero, FStatic::Zero));
	SpringArmComponent -> bUsePawnControlRotation = true;
	CameraComponent -> SetupAttachment(SpringArmComponent);
	CameraComponent -> SetRelativeLocation(FVector::ZeroVector);
	CameraComponent -> bUsePawnControlRotation = true;// 第三人称时改为false

	SetReplicates(true);
	bNetLoadOnClient = true;
	// AutoPossessPlayer = EAutoReceiveInput::Player0;
}

void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();
	SetReplicateMovement(true);
	MainUI = UI -> GetInstance<UMain>(*FStatic::MainBP);
	PopLayer = UI -> GetInstance<UPopLayer>(*FStatic::PopLayerBP);
	PopNotice = UI -> GetInstance<UPopNotice>(*FStatic::PopNoticeBP);
	MainUI -> Open();
	MainUI -> HealthBar -> SetPercent(Health / FStatic::Hundred);
	Local = GetGameInstance() -> GetSubsystem<ULocal>();
	Global = GetWorld() -> GetGameState<AGlobal>();
	GetCharacterMovement() -> NavAgentProps . bCanCrouch = true;
	SpringArmComponent -> TargetArmLength = FStatic::Zero;
	UClass* AnimationClass = LoadObject<UClass>(nullptr, *FStatic::MainAnim);
	if (AnimationClass) {
		GetMesh() -> SetAnimInstanceClass(AnimationClass);
	}
	Join();
}

void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMainCharacter::Join()
{
	if (GetLocalRole() != ROLE_AutonomousProxy || GetNetMode() == NM_DedicatedServer) {
		return;
	}
	FUser User;
	User . UID = Local -> UID;
	User . CID = Local -> CID;
	User . KeyPre = Local -> KeyPre;
	User . NickName = Local -> NickName;
	User . Token = Local -> Token;
	AddPlayerToServer(*Local -> Token, User);
}

void AMainCharacter::AddPlayerToServer_Implementation(const FString& Key, FUser Value)
{
	Global -> Joint . Emplace(Key, Value);
	FLib::Echo(TEXT("服务器保存玩家数据成功！目前人数为 ： " + FString::SanitizeFloat(Global -> Joint . Num())));
	JoinBroadcast(Value . Token, Value . NickName);
}

void AMainCharacter::JoinBroadcast_Implementation(const FString& Token, const FString& NickName)
{
	if (Token . IsEmpty()) {
		return;
	}
	if (GetNetMode() == NM_DedicatedServer) {
		FLib::Echo(TEXT("服务器端 --- 新的加入: " + NickName + " !"));
		return;
	}
	PopNotice -> Pop(NickName + Lang . Get(FStatic::EnteringServerText));
}
