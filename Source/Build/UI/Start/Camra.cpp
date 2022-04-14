#include "Camera.h"

ACamera::ACamera()
{
	PrimaryActorTick . bCanEverTick = false;
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent -> SetRelativeLocation(FVector(0, 0, 0));
	CameraComponent -> SetupAttachment(RootComponent);
	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

void ACamera::BeginPlay()
{
	SetActorLocation(FVector(-4000, 0, 20));
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GWorld, 0);
	PlayerController -> bShowMouseCursor = true;
	PlayerController -> SetIgnoreLookInput(true);
}

void ACamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
