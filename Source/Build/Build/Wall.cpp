#include "Wall.h"

AWall::AWall()
{
	PrimaryActorTick . bCanEverTick = true;

	HalfYZ = FStatic::TwoHundred;
	HalfX = FStatic::Five;
	XYZScale = FStatic::Fifty;
	OverlapCount = FStatic::Zero;
	AttachCount = FStatic::Zero;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(*FStatic::BoxComponent);
	BoxComponent -> InitBoxExtent(FVector(FStatic::Five, FStatic::HundredAndNinety, FStatic::HundredAndNinety));
	SetRootComponent(BoxComponent);

	FrontBoxComponent = CreateDefaultSubobject<UBoxComponent>(*FStatic::Front);
	FrontBoxComponent -> InitBoxExtent(FVector(FStatic::Ten, FStatic::TwoHundred, FStatic::TwoHundred));
	FrontBoxComponent -> SetRelativeLocation(FVector(-FStatic::Hundred, FStatic::Zero, FStatic::Zero));
	FrontBoxComponent -> SetupAttachment(RootComponent);

	BackBoxComponent = CreateDefaultSubobject<UBoxComponent>(*FStatic::Back);
	BackBoxComponent -> InitBoxExtent(FVector(FStatic::Ten, FStatic::TwoHundred, FStatic::TwoHundred));
	BackBoxComponent -> SetRelativeLocation(FVector(FStatic::Hundred, FStatic::Zero, FStatic::Zero));
	BackBoxComponent -> SetupAttachment(RootComponent);

	LowSideBoxComponent = CreateDefaultSubobject<UBoxComponent>(*FStatic::Low);
	LowSideBoxComponent -> InitBoxExtent(FVector(FStatic::Ten, FStatic::HundredAndFifty, FStatic::EightyFive));
	LowSideBoxComponent -> SetRelativeLocation(FVector(FStatic::Zero, FStatic::Zero, -FStatic::HundredAndTwentyFive));
	LowSideBoxComponent -> SetupAttachment(RootComponent);

	UpSideBoxComponent = CreateDefaultSubobject<UBoxComponent>(*FStatic::Up);
	UpSideBoxComponent -> InitBoxExtent(FVector(FStatic::Ten, FStatic::HundredAndFifty, FStatic::EightyFive));
	UpSideBoxComponent -> SetRelativeLocation(FVector(FStatic::Zero, FStatic::Zero, FStatic::HundredAndTwentyFive));
	UpSideBoxComponent -> SetupAttachment(RootComponent);


	static ConstructorHelpers::FObjectFinder<UStaticMesh> Floor(*FStatic::CubeStaticMesh);
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(*FStatic::StaticMeshComponent);
	StaticMeshComponent -> SetupAttachment(RootComponent);
	StaticMeshComponent -> SetStaticMesh(Floor . Object);
	StaticMeshComponent -> SetRelativeScale3D(FVector(HalfX / XYZScale, HalfYZ / XYZScale, HalfYZ / XYZScale));
	StaticMeshComponent -> SetRelativeLocation(FVector(FStatic::Zero, FStatic::Zero, -HalfYZ));
}

void AWall::BeginPlay()
{
	Super::BeginPlay();
	this -> SetMaterial(FStatic::BlurMaterial);
	BoxComponent -> OnComponentBeginOverlap . AddDynamic(this, &AWall::OnOverlapBegin);
	BoxComponent -> OnComponentEndOverlap . AddDynamic(this, &AWall::OnOverlapEnd);

	LowSideBoxComponent -> OnComponentBeginOverlap . AddDynamic(this, &AWall::LowOverlapBegin);
	LowSideBoxComponent -> OnComponentEndOverlap . AddDynamic(this, &AWall::LowOverlapEnd);
	UpSideBoxComponent -> OnComponentBeginOverlap . AddDynamic(this, &AWall::UpOverlapBegin);
	UpSideBoxComponent -> OnComponentEndOverlap . AddDynamic(this, &AWall::UpOverlapEnd);
}

void AWall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWall::SetCollision(const ECollisionEnabled::Type Type) const
{
	StaticMeshComponent -> SetCollisionEnabled(Type);
	if (Type == ECollisionEnabled::QueryOnly) {
		StaticMeshComponent -> SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	} else {
		StaticMeshComponent -> SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	}
}

void AWall::SetMaterial(const FString Value) const
{
	UMaterialInterface* Material = LoadObject<UMaterialInterface>(nullptr, *Value);
	StaticMeshComponent -> SetMaterial(FStatic::Zero, Material);
}

void AWall::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
                           class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                           const FHitResult& SweepResult)
{
	if (!IsSet && FStr::IsOverlapContain(OtherComp -> GetName())) {
		OverlapCount += FStatic::One;
		IsBlock = true;
	}
}

void AWall::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
                         class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!IsSet && FStr::IsOverlapContain(OtherComp -> GetName())) {
		OverlapCount -= FStatic::One;
		if (OverlapCount <= FStatic::Zero) {
			IsBlock = false;
		}
	}
}

void AWall::LowOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
                            class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                            const FHitResult& SweepResult)
{
	if (Save(OtherActor -> GetName(), OtherComp -> GetName())) {
		Low = true;
	}
}

void AWall::LowOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
                          class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (Remove(OtherActor -> GetName(), OtherComp -> GetName())) {
		Low = false;
	}
}

void AWall::UpOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
                           class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                           const FHitResult& SweepResult)
{
	if (Save(OtherActor -> GetName(), OtherComp -> GetName())) {
		Up = true;
	}
}

void AWall::UpOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
                         class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (Remove(OtherActor -> GetName(), OtherComp -> GetName())) {
		Up = false;
	}
}

bool AWall::Save(FString Name, FString CompName)
{
	if (IsSet || !FStr::IsBuildContain(Name) || Name == GetName() || !FStr::IsSideContain(CompName)) {
		return false;
	}
	AttachCount += FStatic::One;
	IsAttach = true;
	if (FStr::IsContain(Name, FStatic::Foundation)) {
		BlockFoundationName = Name;
		BlockFoundationSide = CompName;
	}
	if (FStr::IsContain(Name, FStatic::Wall) && BlockWallName . IsEmpty()) {
		BlockWallName = Name;
		BlockWallSide = CompName;
	}
	FBlockActor BlockActor;
	if (!BlockSideCache . Contains(Name)) {
		BlockActor . Name = Name;
		BlockSideCache . Emplace(Name, FLib::SetSide(CompName, BlockActor));
		return true;
	}
	BlockActor = BlockSideCache[Name];
	BlockSideCache . Emplace(Name, FLib::SetSide(CompName, BlockActor));
	return true;
}

bool AWall::Remove(FString Name, FString CompName)
{
	if (IsSet || BlockSideCache . IsEmpty() || !FStr::IsBuildContain(Name) || !FStr::IsSideContain(CompName)) {
		return false;
	}
	AttachCount -= FStatic::One;
	if (AttachCount <= FStatic::Zero) {
		IsAttach = false;
		BlockFoundationName = nullptr;
		BlockFoundationSide = nullptr;
		BlockWallName = nullptr;
		BlockWallSide = nullptr;
	}
	if (!BlockSideCache . Contains(Name)) {
		return false;
	}
	FBlockActor BlockActor = FLib::SetSide(CompName, BlockSideCache[Name], true);
	if (!BlockActor . Right && !BlockActor . Low && !BlockActor . Left && !BlockActor . Up) {
		BlockSideCache . Remove(Name);
	} else {
		BlockSideCache . Emplace(Name, BlockActor);
	}
	return true;
}
