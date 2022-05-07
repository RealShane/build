#include "Camera.h"

ACamera::ACamera()
{
	PrimaryActorTick . bCanEverTick = false;
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(*FStatic::CameraComponent);
	Request = CreateDefaultSubobject<URequest>(*FStatic::Request);
	CameraComponent -> SetRelativeLocation(FVector::ZeroVector);
	CameraComponent -> SetupAttachment(RootComponent);
	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

void ACamera::BeginPlay()
{
	SetActorLocation(FVector(FStatic::Zero, FStatic::Zero, FStatic::Twenty));
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GWorld, FStatic::Zero);
	PlayerController -> bShowMouseCursor = true;
	PlayerController -> SetIgnoreLookInput(true);
	Request -> Send(FStatic::Test, FStatic::Post);
}

void ACamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACamera::Start(const UWorld* World)
{
	const FSoftObjectPath Path(*FStatic::DemoMap);
	const TSoftObjectPtr<UWorld> Map(Path);
	UGameplayStatics::OpenLevelBySoftObjectPtr(World, Map);
}

void ACamera::Quit() const
{
	UKismetSystemLibrary::QuitGame(this, nullptr, EQuitPreference::Quit, true);
}
