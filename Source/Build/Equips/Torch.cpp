#include "Torch.h"

ATorch::ATorch()
{
	PrimaryActorTick . bCanEverTick = false;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> Torch(*FStatic::Torch);
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(*FStatic::StaticMeshComponent);
	StaticMeshComponent -> SetStaticMesh(Torch . Object);
	StaticMeshComponent -> SetupAttachment(RootComponent);
	StaticMeshComponent -> SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	SetRootComponent(StaticMeshComponent);

	ParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(*FStatic::ParticleSystemComponent);
	static ConstructorHelpers::FObjectFinder<UParticleSystem> Fire(*FStatic::TorchFire);
	ParticleSystemComponent -> SetTemplate(Fire . Object);
	ParticleSystemComponent -> SetRelativeLocation(FVector(FStatic::Zero, FStatic::Zero, FStatic::Hundred));
	ParticleSystemComponent -> SetupAttachment(RootComponent);

	PointLightComponent = CreateDefaultSubobject<UPointLightComponent>(*FStatic::PointLightComponent);
	PointLightComponent -> SetupAttachment(RootComponent);
	PointLightComponent -> SetRelativeLocation(FVector(FStatic::Zero, FStatic::Zero, FStatic::HundredAndTwenty));
}

void ATorch::BeginPlay()
{
	Super::BeginPlay();
}

void ATorch::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATorch::LightOn() const
{
	PointLightComponent -> SetLightColor(FStatic::SunYellowColor);
	PointLightComponent -> SetIntensity(FStatic::FiftyThousand);
	PointLightComponent -> SetSourceLength(FStatic::Hundred);
	PointLightComponent -> SetSourceRadius(FStatic::Hundred);
}
