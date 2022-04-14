#include "Torch.h"

ATorch::ATorch()
{
	PrimaryActorTick . bCanEverTick = false;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> Torch(
		TEXT("StaticMesh'/Game/StarterContent/Props/SM_Statue.SM_Statue'"));
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent -> SetStaticMesh(Torch . Object);
	StaticMeshComponent -> SetupAttachment(RootComponent);
	StaticMeshComponent -> SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	SetRootComponent(StaticMeshComponent);

	ParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystemComponent"));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> Fire(
		TEXT("ParticleSystem'/Game/StarterContent/Particles/P_Fire.P_Fire'"));
	ParticleSystemComponent -> SetTemplate(Fire . Object);
	ParticleSystemComponent -> SetRelativeLocation(FVector(0, 0, 100));
	ParticleSystemComponent -> SetupAttachment(RootComponent);

	PointLightComponent = CreateDefaultSubobject<UPointLightComponent>(TEXT("PointLightComponent"));
	PointLightComponent -> SetupAttachment(RootComponent);
	PointLightComponent -> SetRelativeLocation(FVector(0, 0, 120));
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
	const FLinearColor Color = FColor(243, 159, 024);
	PointLightComponent -> SetLightColor(Color);
	PointLightComponent -> SetIntensity(50000);
	PointLightComponent -> SetSourceLength(100);
	PointLightComponent -> SetSourceRadius(100);
}
