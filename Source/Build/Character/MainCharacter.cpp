// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"

// Sets default values
AMainCharacter::AMainCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	this -> Construct();
	this -> CreateModel();
	this -> ThirdPerson();
	// this -> FirstPerson();
	AutoPossessPlayer = EAutoReceiveInput::Player0;
	
}

void AMainCharacter::Construct()
{
	SkeletalMesh = LoadObject<USkeletalMesh>(NULL, TEXT("SkeletalMesh'/Game/Mannequin/Character/Mesh/SK_Mannequin_Female.SK_Mannequin_Female'"));
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	GetCharacterMovement() -> bOrientRotationToMovement = true;
	GetCharacterMovement() -> JumpZVelocity = 1200;
	GetCharacterMovement() -> AirControl = 1;
	GetCharacterMovement() -> GravityScale = 20;
}

void AMainCharacter::CreateModel()
{
	GetMesh() -> SetRelativeRotation(FRotator(0, -90, 0));
	GetMesh() -> SetRelativeLocation(FVector(0, 0, -100));
	GetMesh() -> SetSkeletalMesh(SkeletalMesh);
	GetCapsuleComponent() -> InitCapsuleSize(42, 100);
}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();
	this -> AnimPlay(Idle, true);
	SetActorLocation(FVector(4000, 4000, 1000));
	GetCharacterMovement() -> NavAgentProps.bCanCrouch = true;
	SpringArmComponent -> TargetArmLength = 600;
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent -> BindAxis("LookRight", this, &AMainCharacter::AddControllerYawInput);
	PlayerInputComponent -> BindAxis("LookForward", this, &AMainCharacter::AddControllerPitchInput);
}

void AMainCharacter::AnimPlay(FString Value, bool loop)
{
	AnimSequence = LoadObject<UAnimSequence>(NULL, *Value);
	GetMesh() -> SetAnimationMode(EAnimationMode::AnimationSingleNode);
	GetMesh() -> SetAnimation(AnimSequence);
	GetMesh() -> Play(loop);
}

FString AMainCharacter::GetPlayingAnimName()
{
	return AnimSequence -> GetPathName();
}

void AMainCharacter::FirstPerson()
{
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;
	CameraComponent -> SetupAttachment(GetMesh());
	CameraComponent -> SetRelativeLocation(FVector(-2.5, -10, 140));
	CameraComponent -> SetRelativeRotation(FRotator(0, 90, 0));
	CameraComponent -> bUsePawnControlRotation = true;
}

void AMainCharacter::ThirdPerson()
{
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	GetCharacterMovement() -> RotationRate = FRotator(0, 540, 0);
	SpringArmComponent -> SetupAttachment(RootComponent);
	SpringArmComponent -> SetRelativeLocation(FVector(0, 0, 80));
	SpringArmComponent -> SetRelativeRotation(FRotator(-15, 0, 0));
	SpringArmComponent -> bUsePawnControlRotation = true;
	CameraComponent -> SetupAttachment(SpringArmComponent);
	CameraComponent -> SetRelativeLocation(FVector::ZeroVector);
	CameraComponent -> bUsePawnControlRotation = false;
}
