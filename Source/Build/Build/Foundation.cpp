#include "Foundation.h"

AFoundation::AFoundation()
{
	PrimaryActorTick . bCanEverTick = true;

	HalfXY = FStatic::TwoHundred;
	HalfZ = FStatic::Thirty;
	XYZScale = FStatic::Fifty;
	OverlapCount = FStatic::Zero;
	AttachCount = FStatic::Zero;
	LandZ = FStatic::Zero;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(*FStatic::BoxComponent);
	BoxComponent -> InitBoxExtent(FVector(FStatic::HundredAndNinety, FStatic::HundredAndNinety, FStatic::Thirty));
	SetRootComponent(BoxComponent);

	RangeBoxComponent = CreateDefaultSubobject<UBoxComponent>(*FStatic::Range);
	RangeBoxComponent -> InitBoxExtent(FVector(FStatic::EightHundred, FStatic::EightHundred, FStatic::TenThousand));
	RangeBoxComponent ->
		SetRelativeLocation(FVector(FStatic::Zero, FStatic::Zero, FStatic::NineThousandAndNineHundred));
	RangeBoxComponent -> SetupAttachment(RootComponent);

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

void AFoundation::BeginPlay()
{
	Super::BeginPlay();
	this -> SetMaterial(FStatic::BlurMaterial);
	BoxComponent -> OnComponentBeginOverlap . AddDynamic(this, &AFoundation::OnOverlapBegin);
	BoxComponent -> OnComponentEndOverlap . AddDynamic(this, &AFoundation::OnOverlapEnd);

	RightSideBoxComponent -> OnComponentBeginOverlap . AddDynamic(this, &AFoundation::RightOverlapBegin);
	RightSideBoxComponent -> OnComponentEndOverlap . AddDynamic(this, &AFoundation::RightOverlapEnd);
	LowSideBoxComponent -> OnComponentBeginOverlap . AddDynamic(this, &AFoundation::LowOverlapBegin);
	LowSideBoxComponent -> OnComponentEndOverlap . AddDynamic(this, &AFoundation::LowOverlapEnd);
	LeftSideBoxComponent -> OnComponentBeginOverlap . AddDynamic(this, &AFoundation::LeftOverlapBegin);
	LeftSideBoxComponent -> OnComponentEndOverlap . AddDynamic(this, &AFoundation::LeftOverlapEnd);
	UpSideBoxComponent -> OnComponentBeginOverlap . AddDynamic(this, &AFoundation::UpOverlapBegin);
	UpSideBoxComponent -> OnComponentEndOverlap . AddDynamic(this, &AFoundation::UpOverlapEnd);
}

void AFoundation::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	this -> LandHeight();
}

void AFoundation::SetCollision(const ECollisionEnabled::Type Type) const
{
	StaticMeshComponent -> SetCollisionEnabled(Type);
	if (Type == ECollisionEnabled::QueryOnly) {
		StaticMeshComponent -> SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	} else {
		StaticMeshComponent -> SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	}
}

void AFoundation::SetMaterial(const FString Value) const
{
	UMaterialInterface* Material = LoadObject<UMaterialInterface>(nullptr, *Value);
	StaticMeshComponent -> SetMaterial(FStatic::Zero, Material);
}

void AFoundation::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
                                 class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                 const FHitResult& SweepResult)
{
	if (!IsSet && FStr::IsOverlapContain(OtherComp -> GetName())) {
		OverlapCount += FStatic::One;
		IsBlock = true;
	}
}

void AFoundation::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
                               class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!IsSet && FStr::IsOverlapContain(OtherComp -> GetName())) {
		OverlapCount -= FStatic::One;
		if (OverlapCount <= FStatic::Zero) {
			IsBlock = false;
		}
	}
}

void AFoundation::RightOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
                                    class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                    const FHitResult& SweepResult)
{
	if (Save(OtherActor -> GetName(), OtherComp -> GetName())) {
		Right = true;
	}
}

void AFoundation::RightOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
                                  class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (Remove(OtherActor -> GetName(), OtherComp -> GetName())) {
		Right = false;
	}
}

void AFoundation::LowOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
                                  class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                  const FHitResult& SweepResult)
{
	if (Save(OtherActor -> GetName(), OtherComp -> GetName())) {
		Low = true;
	}
}

void AFoundation::LowOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
                                class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (Remove(OtherActor -> GetName(), OtherComp -> GetName())) {
		Low = false;
	}
}

void AFoundation::LeftOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
                                   class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                   const FHitResult& SweepResult)
{
	if (Save(OtherActor -> GetName(), OtherComp -> GetName())) {
		Left = true;
	}
}

void AFoundation::LeftOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
                                 class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (Remove(OtherActor -> GetName(), OtherComp -> GetName())) {
		Left = false;
	}
}

void AFoundation::UpOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
                                 class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                 const FHitResult& SweepResult)
{
	if (Save(OtherActor -> GetName(), OtherComp -> GetName())) {
		Up = true;
	}
}

void AFoundation::UpOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
                               class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (Remove(OtherActor -> GetName(), OtherComp -> GetName())) {
		Up = false;
	}
}

bool AFoundation::Save(FString Name, FString CompName)
{
	if (IsSet || !FStr::IsBuildContain(Name) || Name == GetName() || !FStr::IsSideContain(CompName)) {
		return false;
	}
	AttachCount += FStatic::One;
	IsAttach = true;
	BlockActorName = Name;
	BlockActorSide = CompName;
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

bool AFoundation::Remove(FString Name, FString CompName)
{
	if (IsSet || BlockSideCache . IsEmpty() || !FStr::IsBuildContain(Name) || !FStr::IsSideContain(CompName)) {
		return false;
	}
	AttachCount -= FStatic::One;
	if (AttachCount <= FStatic::Zero) {
		IsAttach = false;
		BlockActorName = nullptr;
		BlockActorSide = nullptr;
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

void AFoundation::LandHeight()
{
	/**
	 * DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 1, 0, 5);
	 */
	LandZ = FLib::Max(this -> RayMax(true), this -> RayMax(false));
}

float AFoundation::RayMax(bool IsUp) const
{
	FVector Direction = IsUp ? GetActorUpVector() : -GetActorUpVector();
	FHitResult Center;
	FHitResult RightUp;
	FHitResult RightDown;
	FHitResult LeftUp;
	FHitResult LeftDown;
	FVector CenterStart = GetActorLocation();
	FVector RightUpStart = GetActorLocation() + FVector(FStatic::TwoHundred, FStatic::TwoHundred, FStatic::Zero);
	FVector RightDownStart = GetActorLocation() + FVector(-FStatic::TwoHundred, FStatic::TwoHundred, FStatic::Zero);
	FVector LeftUpStart = GetActorLocation() + FVector(FStatic::TwoHundred, -FStatic::TwoHundred, FStatic::Zero);
	FVector LeftDownStart = GetActorLocation() + FVector(-FStatic::TwoHundred, -FStatic::TwoHundred, FStatic::Zero);
	GetWorld() -> LineTraceSingleByObjectType(Center, CenterStart, ((Direction * FStatic::TenThousand) + CenterStart),
	                                          ECC_WorldStatic);
	GetWorld() -> LineTraceSingleByObjectType(RightUp, RightUpStart,
	                                          ((Direction * FStatic::TenThousand) + RightUpStart), ECC_WorldStatic);
	GetWorld() -> LineTraceSingleByObjectType(RightDown, RightDownStart,
	                                          ((Direction * FStatic::TenThousand) + RightDownStart), ECC_WorldStatic);
	GetWorld() -> LineTraceSingleByObjectType(LeftUp, LeftUpStart, ((Direction * FStatic::TenThousand) + LeftUpStart),
	                                          ECC_WorldStatic);
	GetWorld() -> LineTraceSingleByObjectType(LeftDown, LeftDownStart,
	                                          ((Direction * FStatic::TenThousand) + LeftDownStart), ECC_WorldStatic);
	float CenterZ = FStatic::Zero;
	float RightUpZ = FStatic::Zero;
	float RightDownZ = FStatic::Zero;
	float LeftUpZ = FStatic::Zero;
	float LeftDownZ = FStatic::Zero;
	if (Center . GetComponent() && Center . Location . Z > FStatic::Zero && FStr::IsContain(
		Center . GetComponent() -> GetName(), FStatic::Land)) {
		CenterZ = Center . Location . Z;
	}
	if (RightUp . GetComponent() && RightUp . Location . Z > FStatic::Zero && FStr::IsContain(
		RightUp . GetComponent() -> GetName(), FStatic::Land)) {
		RightUpZ = RightUp . Location . Z;
	}
	if (RightDown . GetComponent() && RightDown . Location . Z > FStatic::Zero && FStr::IsContain(
		RightDown . GetComponent() -> GetName(), FStatic::Land)) {
		RightDownZ = RightDown . Location . Z;
	}
	if (LeftUp . GetComponent() && LeftUp . Location . Z > FStatic::Zero && FStr::IsContain(
		LeftUp . GetComponent() -> GetName(), FStatic::Land)) {
		LeftUpZ = LeftUp . Location . Z;
	}
	if (LeftDown . GetComponent() && LeftDown . Location . Z > FStatic::Zero && FStr::IsContain(
		LeftDown . GetComponent() -> GetName(), FStatic::Land)) {
		LeftDownZ = LeftDown . Location . Z;
	}
	return FLib::Max(FLib::Max(FLib::Max(RightUpZ, RightDownZ), FLib::Max(LeftUpZ, LeftDownZ)), CenterZ);
}
