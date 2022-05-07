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
	RightSideBoxComponent -> InitBoxExtent(FVector(FStatic::Hundred, FStatic::FiftyOne, FStatic::ThirtyOne));
	RightSideBoxComponent -> SetRelativeLocation(FVector(FStatic::Zero, FStatic::HundredAndFiftyOne, FStatic::Zero));
	RightSideBoxComponent -> SetupAttachment(RootComponent);

	LowSideBoxComponent = CreateDefaultSubobject<UBoxComponent>(*FStatic::Low);
	LowSideBoxComponent -> InitBoxExtent(FVector(FStatic::FiftyOne, FStatic::Hundred, FStatic::ThirtyOne));
	LowSideBoxComponent -> SetRelativeLocation(FVector(-FStatic::HundredAndFiftyOne, FStatic::Zero, FStatic::Zero));
	LowSideBoxComponent -> SetupAttachment(RootComponent);

	LeftSideBoxComponent = CreateDefaultSubobject<UBoxComponent>(*FStatic::Left);
	LeftSideBoxComponent -> InitBoxExtent(FVector(FStatic::Hundred, FStatic::FiftyOne, FStatic::ThirtyOne));
	LeftSideBoxComponent -> SetRelativeLocation(FVector(FStatic::Zero, -FStatic::HundredAndFiftyOne, FStatic::Zero));
	LeftSideBoxComponent -> SetupAttachment(RootComponent);

	UpSideBoxComponent = CreateDefaultSubobject<UBoxComponent>(*FStatic::Up);
	UpSideBoxComponent -> InitBoxExtent(FVector(FStatic::FiftyOne, FStatic::Hundred, FStatic::ThirtyOne));
	UpSideBoxComponent -> SetRelativeLocation(FVector(FStatic::HundredAndFiftyOne, FStatic::Zero, FStatic::Zero));
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
	if (!IsSet) {
		this -> LandHeight();
		this -> RightDetectRay();
		this -> LowDetectRay();
		this -> LeftDetectRay();
		this -> UpDetectRay();
	}
}

void AFoundation::SetCollision(const ECollisionEnabled::Type Type) const
{
	StaticMeshComponent -> SetCollisionEnabled(Type);
	if (Type == ECollisionEnabled::QueryOnly) {
		StaticMeshComponent -> SetCollisionResponseToAllChannels(ECR_Ignore);
	} else {
		StaticMeshComponent -> SetCollisionResponseToAllChannels(ECR_Block);
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
	if (IsSet || !FStr::IsOverlapContain(OtherComp -> GetName())) {
		return;
	}
	OverlapCount += FStatic::One;
	IsBlock = true;
}

void AFoundation::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
                               class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (IsSet || !FStr::IsOverlapContain(OtherComp -> GetName())) {
		return;
	}
	OverlapCount -= FStatic::One;
	if (OverlapCount > FStatic::Zero) {
		return;
	}
	IsBlock = false;
}

void AFoundation::RightOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
                                    class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                    const FHitResult& SweepResult)
{
	if (FStr::IsContain(OtherActor -> GetName(), FStatic::Wall) &&
		FStr::IsContain(OtherComp -> GetName(), FStatic::Low)) {
		WallRight = true;
	}
}

void AFoundation::RightOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
                                  class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (FStr::IsContain(OtherActor -> GetName(), FStatic::Wall) &&
		FStr::IsContain(OtherComp -> GetName(), FStatic::Low)) {
		WallRight = false;
	}
}

void AFoundation::LowOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
                                  class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                  const FHitResult& SweepResult)
{
	if (FStr::IsContain(OtherActor -> GetName(), FStatic::Wall) &&
		FStr::IsContain(OtherComp -> GetName(), FStatic::Low)) {
		WallLow = true;
	}
}

void AFoundation::LowOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
                                class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (FStr::IsContain(OtherActor -> GetName(), FStatic::Wall) &&
		FStr::IsContain(OtherComp -> GetName(), FStatic::Low)) {
		WallLow = false;
	}
}

void AFoundation::LeftOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
                                   class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                   const FHitResult& SweepResult)
{
	if (FStr::IsContain(OtherActor -> GetName(), FStatic::Wall) &&
		FStr::IsContain(OtherComp -> GetName(), FStatic::Low)) {
		WallLeft = true;
	}
}

void AFoundation::LeftOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
                                 class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (FStr::IsContain(OtherActor -> GetName(), FStatic::Wall) &&
		FStr::IsContain(OtherComp -> GetName(), FStatic::Low)) {
		WallLeft = false;
	}
}

void AFoundation::UpOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
                                 class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                 const FHitResult& SweepResult)
{
	if (FStr::IsContain(OtherActor -> GetName(), FStatic::Wall) &&
		FStr::IsContain(OtherComp -> GetName(), FStatic::Low)) {
		WallUp = true;
	}
}

void AFoundation::UpOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
                               class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (FStr::IsContain(OtherActor -> GetName(), FStatic::Wall) &&
		FStr::IsContain(OtherComp -> GetName(), FStatic::Low)) {
		WallUp = false;
	}
}

void AFoundation::RightDetectRay()
{
	FHitResult Hit;
	FVector Start = GetActorLocation();
	FCollisionQueryParams Params(FName(TEXT("")), false, nullptr);
	Params . AddIgnoredActor(this);
	bool IsHit = GetWorld() -> LineTraceSingleByObjectType(Hit, Start,
	                                                       GetActorRightVector() * FStatic::TwoHundredAndOne + Start,
	                                                       ECC_WorldStatic, Params);
	if (IsHit) {
		FString Name = Hit . GetActor() -> GetName();
		FString CompName = Hit . GetComponent() -> GetName();
		if (!FStr::IsContain(Name, FStatic::Foundation) || !FStr::IsSideContain(CompName)) {
			return;
		}
		if (Save(Name, CompName)) {
			RightDetectName = Name;
			RightDetectCompName = CompName;
			Right = true;
		}
		return;
	}
	if (!RightDetectName . IsEmpty()) {
		if (Remove(RightDetectName, RightDetectCompName)) {
			RightDetectName = nullptr;
			RightDetectCompName = nullptr;
			Right = false;
		}
	}
}

void AFoundation::LowDetectRay()
{
	FHitResult Hit;
	FVector Start = GetActorLocation();
	FCollisionQueryParams Params(FName(TEXT("")), false, nullptr);
	Params . AddIgnoredActor(this);
	bool IsHit = GetWorld() -> LineTraceSingleByObjectType(Hit, Start,
	                                                       -GetActorForwardVector() * FStatic::TwoHundredAndOne +
	                                                       Start,
	                                                       ECC_WorldStatic, Params);
	if (IsHit) {
		FString Name = Hit . GetActor() -> GetName();
		FString CompName = Hit . GetComponent() -> GetName();
		if (!FStr::IsContain(Name, FStatic::Foundation) || !FStr::IsSideContain(CompName)) {
			return;
		}
		if (Save(Name, CompName)) {
			LowDetectName = Name;
			LowDetectCompName = CompName;
			Low = true;
		}
		return;
	}
	if (!LowDetectName . IsEmpty()) {
		if (Remove(LowDetectName, LowDetectCompName)) {
			LowDetectName = nullptr;
			LowDetectCompName = nullptr;
			Low = false;
		}
	}
}

void AFoundation::LeftDetectRay()
{
	FHitResult Hit;
	FVector Start = GetActorLocation();
	FCollisionQueryParams Params(FName(TEXT("")), false, nullptr);
	Params . AddIgnoredActor(this);
	bool IsHit = GetWorld() -> LineTraceSingleByObjectType(Hit, Start,
	                                                       -GetActorRightVector() * FStatic::TwoHundredAndOne + Start,
	                                                       ECC_WorldStatic, Params);
	if (IsHit) {
		FString Name = Hit . GetActor() -> GetName();
		FString CompName = Hit . GetComponent() -> GetName();
		if (!FStr::IsContain(Name, FStatic::Foundation) || !FStr::IsSideContain(CompName)) {
			return;
		}
		if (Save(Name, CompName)) {
			LeftDetectName = Name;
			LeftDetectCompName = CompName;
			Left = true;
		}
		return;
	}
	if (!LeftDetectName . IsEmpty()) {
		if (Remove(LeftDetectName, LeftDetectCompName)) {
			LeftDetectName = nullptr;
			LeftDetectCompName = nullptr;
			Left = false;
		}
	}
}

void AFoundation::UpDetectRay()
{
	FHitResult Hit;
	FVector Start = GetActorLocation();
	FCollisionQueryParams Params(FName(TEXT("")), false, nullptr);
	Params . AddIgnoredActor(this);
	bool IsHit = GetWorld() -> LineTraceSingleByObjectType(Hit, Start,
	                                                       GetActorForwardVector() * FStatic::TwoHundredAndOne +
	                                                       Start,
	                                                       ECC_WorldStatic, Params);
	if (IsHit) {
		FString Name = Hit . GetActor() -> GetName();
		FString CompName = Hit . GetComponent() -> GetName();
		if (!FStr::IsContain(Name, FStatic::Foundation) || !FStr::IsSideContain(CompName)) {
			return;
		}
		if (Save(Name, CompName)) {
			UpDetectName = Name;
			UpDetectCompName = CompName;
			Up = true;
		}
		return;
	}
	if (!UpDetectName . IsEmpty()) {
		if (Remove(UpDetectName, UpDetectCompName)) {
			UpDetectName = nullptr;
			UpDetectCompName = nullptr;
			Up = false;
		}
	}
}

bool AFoundation::Save(FString Name, FString CompName)
{
	AttachCount += FStatic::One;
	IsAttach = true;
	FBlockActor BlockActor;
	if (!BlockSideCache . Contains(Name)) {
		BlockSideCache . Emplace(Name, FStr::SetSide(CompName, BlockActor));
		return true;
	}
	BlockActor = BlockSideCache[Name];
	BlockSideCache . Emplace(Name, FStr::SetSide(CompName, BlockActor));
	return true;
}

bool AFoundation::Remove(FString Name, FString CompName)
{
	AttachCount -= FStatic::One;
	if (AttachCount <= FStatic::Zero) {
		IsAttach = false;
	}
	if (!BlockSideCache . Contains(Name)) {
		return true;
	}
	FBlockActor BlockActor = FStr::SetSide(CompName, BlockSideCache[Name], true);
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
	const float UpZ = this -> RayMax(true) + FStatic::Twenty;
	const float DownZ = this -> RayMax(false) + FStatic::Twenty;
	if (FStr::IsContain(UpRayName, FStatic::Foundation)) {
		LandZ = UpZ;
		return;
	}
	LandZ = FLib::Max(UpZ, DownZ);
}

float AFoundation::RayMax(bool IsUp)
{
	FVector Direction = IsUp ? GetActorUpVector() : -GetActorUpVector();
	FHitResult Center;
	FVector CenterStart = GetActorLocation();
	FCollisionQueryParams Params(FName(TEXT("")), false, nullptr);
	Params . AddIgnoredActor(this);
	bool CenterIsHit = GetWorld() -> LineTraceSingleByObjectType(Center, CenterStart,
	                                                             Direction * FStatic::TenThousand + CenterStart,
	                                                             ECC_WorldStatic, Params);
	float CenterZ = FStatic::Zero;
	if (CenterIsHit) {
		CenterZ = Center . Location . Z;
		if (IsUp) {
			UpRayName = Center . GetActor() -> GetName();
		} else {
			DownRayName = Center . GetActor() -> GetName();
		}
	} else {
		if (IsUp) {
			UpRayName = nullptr;
		} else {
			DownRayName = nullptr;
		}
	}
	return CenterZ;
}
