#include "Floor.h"

AFloor::AFloor()
{
	PrimaryActorTick . bCanEverTick = true;
	HalfXY = FStatic::TwoHundred;
	HalfZ = FStatic::Five;
	XYZScale = FStatic::Fifty;
	OverlapCount = FStatic::Zero;
	AttachCount = FStatic::Zero;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(*FStatic::BoxComponent);
	BoxComponent -> InitBoxExtent(FVector(FStatic::HundredAndNinety, FStatic::HundredAndNinety, FStatic::Five));
	SetRootComponent(BoxComponent);

	RightSideBoxComponent = CreateDefaultSubobject<UBoxComponent>(*FStatic::Right);
	RightSideBoxComponent -> InitBoxExtent(FVector(FStatic::Hundred, FStatic::Ten, FStatic::Thirty));
	RightSideBoxComponent -> SetRelativeLocation(FVector(FStatic::Zero, FStatic::TwoHundred, FStatic::Zero));
	RightSideBoxComponent -> SetupAttachment(RootComponent);

	LowSideBoxComponent = CreateDefaultSubobject<UBoxComponent>(*FStatic::Low);
	LowSideBoxComponent -> InitBoxExtent(FVector(FStatic::Ten, FStatic::Hundred, FStatic::Thirty));
	LowSideBoxComponent -> SetRelativeLocation(FVector(-FStatic::TwoHundred, FStatic::Zero, FStatic::Zero));
	LowSideBoxComponent -> SetupAttachment(RootComponent);

	LeftSideBoxComponent = CreateDefaultSubobject<UBoxComponent>(*FStatic::Left);
	LeftSideBoxComponent -> InitBoxExtent(FVector(FStatic::Hundred, FStatic::Ten, FStatic::Thirty));
	LeftSideBoxComponent -> SetRelativeLocation(FVector(FStatic::Zero, -FStatic::TwoHundred, FStatic::Zero));
	LeftSideBoxComponent -> SetupAttachment(RootComponent);

	UpSideBoxComponent = CreateDefaultSubobject<UBoxComponent>(*FStatic::Up);
	UpSideBoxComponent -> InitBoxExtent(FVector(FStatic::Ten, FStatic::Hundred, FStatic::Thirty));
	UpSideBoxComponent -> SetRelativeLocation(FVector(FStatic::TwoHundred, FStatic::Zero, FStatic::Zero));
	UpSideBoxComponent -> SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> Foundation(*FStatic::CubeStaticMesh);
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(*FStatic::StaticMeshComponent);
	StaticMeshComponent -> SetupAttachment(RootComponent);
	StaticMeshComponent -> SetStaticMesh(Foundation . Object);
	StaticMeshComponent -> SetRelativeScale3D(FVector(HalfXY / XYZScale, HalfXY / XYZScale, HalfZ / XYZScale));
	StaticMeshComponent -> SetRelativeLocation(FVector(FStatic::Zero, FStatic::Zero, -HalfZ));
}

void AFloor::BeginPlay()
{
	Super::BeginPlay();
	this -> SetMaterial(FStatic::BlurMaterial);
	BoxComponent -> OnComponentBeginOverlap . AddDynamic(this, &AFloor::OnOverlapBegin);
	BoxComponent -> OnComponentEndOverlap . AddDynamic(this, &AFloor::OnOverlapEnd);

	RightSideBoxComponent -> OnComponentBeginOverlap . AddDynamic(this, &AFloor::RightOverlapBegin);
	RightSideBoxComponent -> OnComponentEndOverlap . AddDynamic(this, &AFloor::RightOverlapEnd);
	LowSideBoxComponent -> OnComponentBeginOverlap . AddDynamic(this, &AFloor::LowOverlapBegin);
	LowSideBoxComponent -> OnComponentEndOverlap . AddDynamic(this, &AFloor::LowOverlapEnd);
	LeftSideBoxComponent -> OnComponentBeginOverlap . AddDynamic(this, &AFloor::LeftOverlapBegin);
	LeftSideBoxComponent -> OnComponentEndOverlap . AddDynamic(this, &AFloor::LeftOverlapEnd);
	UpSideBoxComponent -> OnComponentBeginOverlap . AddDynamic(this, &AFloor::UpOverlapBegin);
	UpSideBoxComponent -> OnComponentEndOverlap . AddDynamic(this, &AFloor::UpOverlapEnd);
}

void AFloor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AFloor::SetCollision(const ECollisionEnabled::Type Type) const
{
	StaticMeshComponent -> SetCollisionEnabled(Type);
	if (Type == ECollisionEnabled::QueryOnly) {
		StaticMeshComponent -> SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	} else {
		StaticMeshComponent -> SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	}
}

void AFloor::SetMaterial(const FString Value) const
{
	UMaterialInterface* Material = LoadObject<UMaterialInterface>(nullptr, *Value);
	StaticMeshComponent -> SetMaterial(FStatic::Zero, Material);
}

void AFloor::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
                            class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                            const FHitResult& SweepResult)
{
	if (!IsSet && FStr::IsOverlapContain(OtherComp -> GetName())) {
		OverlapCount += FStatic::One;
		IsBlock = true;
	}
}

void AFloor::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
                          class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!IsSet && FStr::IsOverlapContain(OtherComp -> GetName())) {
		OverlapCount -= FStatic::One;
		if (OverlapCount <= FStatic::Zero) {
			IsBlock = false;
		}
	}
}

void AFloor::RightOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
                               class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                               const FHitResult& SweepResult)
{
	if (Save(OtherActor -> GetName(), OtherComp -> GetName())) {
		Right = true;
	}
}

void AFloor::RightOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
                             class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (Remove(OtherActor -> GetName(), OtherComp -> GetName())) {
		Right = false;
	}
}

void AFloor::LowOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
                             class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                             const FHitResult& SweepResult)
{
	if (Save(OtherActor -> GetName(), OtherComp -> GetName())) {
		Low = true;
	}
}

void AFloor::LowOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
                           class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (Remove(OtherActor -> GetName(), OtherComp -> GetName())) {
		Low = false;
	}
}

void AFloor::LeftOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
                              class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                              const FHitResult& SweepResult)
{
	if (Save(OtherActor -> GetName(), OtherComp -> GetName())) {
		Left = true;
	}
}

void AFloor::LeftOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
                            class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (Remove(OtherActor -> GetName(), OtherComp -> GetName())) {
		Left = false;
	}
}

void AFloor::UpOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
                            class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                            const FHitResult& SweepResult)
{
	if (Save(OtherActor -> GetName(), OtherComp -> GetName())) {
		Up = true;
	}
}

void AFloor::UpOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
                          class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (Remove(OtherActor -> GetName(), OtherComp -> GetName())) {
		Up = false;
	}
}

bool AFloor::Save(FString Name, FString CompName)
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
	if (FStr::IsContain(Name, FStatic::Floor) && BlockFloorName . IsEmpty()) {
		BlockFloorName = Name;
		BlockFloorSide = CompName;
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

bool AFloor::Remove(FString Name, FString CompName)
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
		BlockFloorName = nullptr;
		BlockFloorSide = nullptr;
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
