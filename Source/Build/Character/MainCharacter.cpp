#include "MainCharacter.h"

AMainCharacter::AMainCharacter()
{
	PrimaryActorTick . bCanEverTick = false;
	Idle = FStatic::Idle;
	Jog = FStatic::Jog;
	JumpAnim = FStatic::JumpAnim;
	JumpLoopAnim = FStatic::JumpLoopAnim;
	this -> Construct();
	this -> CreateModel();
	this -> ThirdPerson();
	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

void AMainCharacter::Construct()
{
	SkeletalMesh = LoadObject<USkeletalMesh>(nullptr, *FStatic::MannequinFemale);
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(*FStatic::SpringArmComponent);
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(*FStatic::CameraComponent);
	GetCharacterMovement() -> bOrientRotationToMovement = true;
	GetCharacterMovement() -> JumpZVelocity = FStatic::ThousandAndTwoHundred;
	GetCharacterMovement() -> AirControl = FStatic::One;
	GetCharacterMovement() -> GravityScale = FStatic::Twenty;
}

void AMainCharacter::CreateModel() const
{
	GetMesh() -> SetRelativeRotation(FRotator(FStatic::Zero, -FStatic::Ninety, FStatic::Zero));
	GetMesh() -> SetRelativeLocation(FVector(FStatic::Zero, FStatic::Zero, -FStatic::Hundred));
	GetMesh() -> SetSkeletalMesh(SkeletalMesh);
	GetCapsuleComponent() -> InitCapsuleSize(FStatic::FortyTwo, FStatic::Hundred);
}

void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();
	this -> AnimPlay(Idle, true);
	SetActorLocation(FVector(FStatic::FourThousand, FStatic::FourThousand, FStatic::Thousand));
	GetCharacterMovement() -> NavAgentProps . bCanCrouch = true;
	SpringArmComponent -> TargetArmLength = FStatic::SixHundred;
}

void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AMainCharacter::AnimPlay(FString Value, bool loop)
{
	AnimSequence = LoadObject<UAnimSequence>(nullptr, *Value);
	GetMesh() -> SetAnimationMode(EAnimationMode::AnimationSingleNode);
	GetMesh() -> SetAnimation(AnimSequence);
	GetMesh() -> Play(loop);
}

FString AMainCharacter::GetPlayingAnimName() const
{
	return AnimSequence -> GetPathName();
}

void AMainCharacter::FirstPerson()
{
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;
	CameraComponent -> SetupAttachment(GetMesh());
	CameraComponent -> SetRelativeLocation(FVector(-FStatic::TwoPointFive, -FStatic::Ten, FStatic::HundredAndForty));
	CameraComponent -> SetRelativeRotation(FRotator(FStatic::Zero, FStatic::Ninety, FStatic::Zero));
	CameraComponent -> bUsePawnControlRotation = true;
}

void AMainCharacter::ThirdPerson()
{
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	GetCharacterMovement() -> RotationRate = FRotator(FStatic::Zero, FStatic::FiveHundredAndForty, FStatic::Zero);
	SpringArmComponent -> SetupAttachment(RootComponent);
	SpringArmComponent -> SetRelativeLocation(FVector(FStatic::Zero, FStatic::Zero, FStatic::Eighty));
	SpringArmComponent -> SetRelativeRotation(FRotator(-FStatic::Fifteen, FStatic::Zero, FStatic::Zero));
	SpringArmComponent -> bUsePawnControlRotation = true;
	CameraComponent -> SetupAttachment(SpringArmComponent);
	CameraComponent -> SetRelativeLocation(FVector::ZeroVector);
	CameraComponent -> bUsePawnControlRotation = false;
}
