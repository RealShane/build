#include "CloneCharacter.h"

ACloneCharacter::ACloneCharacter()
{
	PrimaryActorTick . bCanEverTick = false;
	this -> Construct();
	this -> CreateModel();
}

void ACloneCharacter::Construct()
{
	SkeletalMesh = LoadObject<USkeletalMesh>(nullptr, *FStatic::MannequinFemale);
	GetCharacterMovement() -> bOrientRotationToMovement = true;
	GetCharacterMovement() -> JumpZVelocity = FStatic::ThousandAndTwoHundred;
	GetCharacterMovement() -> AirControl = FStatic::One;
	GetCharacterMovement() -> GravityScale = FStatic::Twenty;
}

void ACloneCharacter::CreateModel() const
{
	GetMesh() -> SetRelativeRotation(FRotator(FStatic::Zero, -FStatic::Ninety, FStatic::Zero));
	GetMesh() -> SetRelativeLocation(FVector(FStatic::Zero, FStatic::Zero, -FStatic::Hundred));
	GetMesh() -> SetSkeletalMesh(SkeletalMesh);
	GetCapsuleComponent() -> InitCapsuleSize(FStatic::FortyTwo, FStatic::Hundred);
	GetCapsuleComponent() -> SetCollisionObjectType(ECC_WorldStatic);
}

void ACloneCharacter::BeginPlay()
{
	Super::BeginPlay();
	this -> AnimPlay(FStatic::Idle, true);
	SetActorLocation(FVector(FStatic::FourThousand, FStatic::FourThousand, FStatic::Thousand));
	GetCharacterMovement() -> NavAgentProps . bCanCrouch = true;
}

void ACloneCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACloneCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ACloneCharacter::AnimPlay(FString Value, bool loop)
{
	AnimSequence = LoadObject<UAnimSequence>(nullptr, *Value);
	GetMesh() -> SetAnimationMode(EAnimationMode::AnimationSingleNode);
	GetMesh() -> SetAnimation(AnimSequence);
	GetMesh() -> Play(loop);
}

FString ACloneCharacter::GetPlayingAnimName() const
{
	return AnimSequence -> GetPathName();
}
