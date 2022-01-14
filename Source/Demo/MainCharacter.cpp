// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"

#include "Lib.h"

// Sets default values
AMainCharacter::AMainCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	this -> Construct();
	this -> CreateModel();
	this -> CreateCamera();
	AutoPossessPlayer = EAutoReceiveInput::Player0;
	
}

void AMainCharacter::Construct()
{
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	SkeletalMesh = LoadObject<USkeletalMesh>(NULL, TEXT("SkeletalMesh'/Game/Mannequin/Character/Mesh/SK_Mannequin_Female.SK_Mannequin_Female'"));
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	GetCharacterMovement() -> bOrientRotationToMovement = true;
	GetCharacterMovement() -> RotationRate = FRotator(0, 540, 0);
	GetCharacterMovement() -> JumpZVelocity = 1200;
	GetCharacterMovement() -> AirControl = 1;
	GetCharacterMovement() -> GravityScale = 20;
}

void AMainCharacter::CreateModel()
{
	GetMesh() -> SetSkeletalMesh(SkeletalMesh);
	GetMesh() -> SetRelativeRotation(FRotator(0, -90, 0));
	GetMesh() -> SetRelativeLocation(FVector(0, 0, -100));
	GetCapsuleComponent() -> InitCapsuleSize(42, 100);
}

void AMainCharacter::CreateCamera()
{
	SpringArmComponent -> SetupAttachment(RootComponent);
	SpringArmComponent -> SetRelativeLocation(FVector(0, 0, 100));
	SpringArmComponent -> SetRelativeRotation(FRotator(-15, 0, 0));
	SpringArmComponent -> TargetArmLength = 600;
	SpringArmComponent -> bUsePawnControlRotation = true;
	CameraComponent -> SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);
	CameraComponent -> bUsePawnControlRotation = false;
}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();
	this -> AnimPlay(Idle, true);
	SetActorLocation(FVector(12000, 12000, 200));
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
