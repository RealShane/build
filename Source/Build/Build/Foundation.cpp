#include "Foundation.h"

AFoundation::AFoundation()
{
	PrimaryActorTick . bCanEverTick = true;

	HalfXY = FStatic::TwoHundred;
	HalfZ = FStatic::Thirty;
	XYZScale = FStatic::Fifty;
	OverlapCount = FStatic::Zero;
	LandZ = FStatic::Zero;

	bReplicates = true;
	bNetLoadOnClient = true;
	bAlwaysRelevant = true;
	// NetCullDistanceSquared = 1000;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(*FStatic::BoxComponent);
	BoxComponent -> InitBoxExtent(FVector(FStatic::HundredAndNinety, FStatic::HundredAndNinety, FStatic::Thirty));
	SetRootComponent(BoxComponent);

	RangeBoxComponent = CreateDefaultSubobject<UBoxComponent>(*FStatic::Range);
	RangeBoxComponent -> InitBoxExtent(FVector::ZeroVector);
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
	StaticMeshComponent -> SetCollisionEnabled(ECollisionEnabled::NoCollision);
	StaticMeshComponent -> SetOnlyOwnerSee(true);
}

void AFoundation::BeginPlay()
{
	Super::BeginPlay();
	SetReplicateMovement(true);
	Local = GetGameInstance() -> GetSubsystem<ULocal>();
	Global = GetWorld() -> GetGameState<AGlobal>();
	SetMaterial(FStatic::BlurMaterial);
	SetCollision(ECollisionEnabled::QueryOnly);
	BoxComponent -> OnComponentBeginOverlap . AddDynamic(this, &ThisClass::OnOverlapBegin);
	BoxComponent -> OnComponentEndOverlap . AddDynamic(this, &ThisClass::OnOverlapEnd);

	RightSideBoxComponent -> OnComponentBeginOverlap . AddDynamic(this, &ThisClass::RightOverlapBegin);
	RightSideBoxComponent -> OnComponentEndOverlap . AddDynamic(this, &ThisClass::RightOverlapEnd);
	LowSideBoxComponent -> OnComponentBeginOverlap . AddDynamic(this, &ThisClass::LowOverlapBegin);
	LowSideBoxComponent -> OnComponentEndOverlap . AddDynamic(this, &ThisClass::LowOverlapEnd);
	LeftSideBoxComponent -> OnComponentBeginOverlap . AddDynamic(this, &ThisClass::LeftOverlapBegin);
	LeftSideBoxComponent -> OnComponentEndOverlap . AddDynamic(this, &ThisClass::LeftOverlapEnd);
	UpSideBoxComponent -> OnComponentBeginOverlap . AddDynamic(this, &ThisClass::UpOverlapBegin);
	UpSideBoxComponent -> OnComponentEndOverlap . AddDynamic(this, &ThisClass::UpOverlapEnd);

	RangeBoxComponent -> OnComponentBeginOverlap . AddDynamic(this, &ThisClass::RangeOverlapBegin);
	RangeBoxComponent -> OnComponentEndOverlap . AddDynamic(this, &ThisClass::RangeOverlapEnd);
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
		if (GetNetMode() == NM_DedicatedServer || GetOwner() -> GetLocalRole() != ROLE_AutonomousProxy) {
			return;
		}
		this -> RangeDetectRay();
	} else {
		PrimaryActorTick . SetTickFunctionEnable(false);
		SetCollision(ECollisionEnabled::QueryAndPhysics);
		SetMaterial(FStatic::WoodMaterial);
		SetRange();
		Tags . Emplace(Key);
		StaticMeshComponent -> SetOnlyOwnerSee(false);
		StaticMeshComponent -> SetMobility(EComponentMobility::Static);
		StaticMeshComponent -> SetCollisionObjectType(ECC_WorldStatic);
		BoxComponent -> SetCollisionObjectType(ECC_WorldStatic);
		RightSideBoxComponent -> SetCollisionObjectType(ECC_WorldStatic);
		LowSideBoxComponent -> SetCollisionObjectType(ECC_WorldStatic);
		LeftSideBoxComponent -> SetCollisionObjectType(ECC_WorldStatic);
		UpSideBoxComponent -> SetCollisionObjectType(ECC_WorldStatic);
	}
}

void AFoundation::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass, Key);
	DOREPLIFETIME(ThisClass, IsSet);
	DOREPLIFETIME(ThisClass, EnemyRange);
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

void AFoundation::SetRange()
{
	RangeBoxComponent -> SetBoxExtent(FVector(FStatic::TwoThousand, FStatic::TwoThousand, FStatic::TenThousand));
	RangeBoxComponent ->
		SetRelativeLocation(FVector(FStatic::Zero, FStatic::Zero, FStatic::NineThousandAndNineHundred));
	RangeBoxComponent -> SetCollisionProfileName(*(FStatic::Foundation + FStatic::Range));
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

void AFoundation::RangeOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                    const FHitResult& SweepResult)
{
}

void AFoundation::RangeOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
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
	if (!HasAuthority()) {
		return;
	}
	FHitResult Hit;
	FVector Start = GetActorLocation();
	FCollisionQueryParams Params;
	Params . AddIgnoredActor(this);
	Params . bTraceComplex = false;
	bool IsHit = GetWorld() -> LineTraceSingleByObjectType(
		Hit, Start,
		GetActorRightVector() * FStatic::TwoHundredAndOne + Start,
		ECC_WorldStatic, Params
	);
	if (IsHit) {
		FString Name = Hit . GetActor() -> GetName();
		FString CompName = Hit . GetComponent() -> GetName();
		if (!FStr::IsContain(Name, FStatic::Foundation) || !FStr::IsSideContain(CompName)) {
			return;
		}
		Name = Hit . GetActor() -> Tags[FStatic::Zero] . ToString();
		Save(Name, CompName);
		IsAttach = true;
		RightDetectName = Name;
		RightDetectCompName = CompName;
		Right = true;
		return;
	}
	if (!RightDetectName . IsEmpty()) {
		Remove(RightDetectName, RightDetectCompName);
		IsAttach = false;
		RightDetectName = nullptr;
		RightDetectCompName = nullptr;
		Right = false;
	}
}

void AFoundation::LowDetectRay()
{
	if (!HasAuthority()) {
		return;
	}
	FHitResult Hit;
	FVector Start = GetActorLocation();
	FCollisionQueryParams Params;
	Params . AddIgnoredActor(this);
	Params . bTraceComplex = false;
	bool IsHit = GetWorld() -> LineTraceSingleByObjectType(
		Hit, Start,
		-GetActorForwardVector() * FStatic::TwoHundredAndOne +
		Start,
		ECC_WorldStatic, Params
	);
	if (IsHit) {
		FString Name = Hit . GetActor() -> GetName();
		FString CompName = Hit . GetComponent() -> GetName();
		if (!FStr::IsContain(Name, FStatic::Foundation) || !FStr::IsSideContain(CompName)) {
			return;
		}
		Name = Hit . GetActor() -> Tags[FStatic::Zero] . ToString();
		Save(Name, CompName);
		IsAttach = true;
		LowDetectName = Name;
		LowDetectCompName = CompName;
		Low = true;
		return;
	}
	if (!LowDetectName . IsEmpty()) {
		Remove(LowDetectName, LowDetectCompName);
		IsAttach = false;
		LowDetectName = nullptr;
		LowDetectCompName = nullptr;
		Low = false;
	}
}

void AFoundation::LeftDetectRay()
{
	if (!HasAuthority()) {
		return;
	}
	FHitResult Hit;
	FVector Start = GetActorLocation();
	FCollisionQueryParams Params;
	Params . AddIgnoredActor(this);
	Params . bTraceComplex = false;
	bool IsHit = GetWorld() -> LineTraceSingleByObjectType(
		Hit, Start,
		-GetActorRightVector() * FStatic::TwoHundredAndOne + Start,
		ECC_WorldStatic, Params
	);
	if (IsHit) {
		FString Name = Hit . GetActor() -> GetName();
		FString CompName = Hit . GetComponent() -> GetName();
		if (!FStr::IsContain(Name, FStatic::Foundation) || !FStr::IsSideContain(CompName)) {
			return;
		}
		Name = Hit . GetActor() -> Tags[FStatic::Zero] . ToString();
		Save(Name, CompName);
		IsAttach = true;
		LeftDetectName = Name;
		LeftDetectCompName = CompName;
		Left = true;
		return;
	}
	if (!LeftDetectName . IsEmpty()) {
		Remove(LeftDetectName, LeftDetectCompName);
		IsAttach = false;
		LeftDetectName = nullptr;
		LeftDetectCompName = nullptr;
		Left = false;
	}
}

void AFoundation::UpDetectRay()
{
	if (!HasAuthority()) {
		return;
	}
	FHitResult Hit;
	FVector Start = GetActorLocation();
	FCollisionQueryParams Params;
	Params . AddIgnoredActor(this);
	Params . bTraceComplex = false;
	bool IsHit = GetWorld() -> LineTraceSingleByObjectType(
		Hit, Start,
		GetActorForwardVector() * FStatic::TwoHundredAndOne +
		Start, ECC_WorldStatic, Params
	);
	if (IsHit) {
		FString Name = Hit . GetActor() -> GetName();
		FString CompName = Hit . GetComponent() -> GetName();
		if (!FStr::IsContain(Name, FStatic::Foundation) || !FStr::IsSideContain(CompName)) {
			return;
		}
		Name = Hit . GetActor() -> Tags[FStatic::Zero] . ToString();
		Save(Name, CompName);
		IsAttach = true;
		UpDetectName = Name;
		UpDetectCompName = CompName;
		Up = true;
		return;
	}
	if (!UpDetectName . IsEmpty()) {
		Remove(UpDetectName, UpDetectCompName);
		IsAttach = false;
		UpDetectName = nullptr;
		UpDetectCompName = nullptr;
		Up = false;
	}
}

void AFoundation::Save_Implementation(const FString& Name, const FString& CompName)
{
	FBlockActor BlockActor;
	if (!BlockSideCache . Contains(Name)) {
		BlockSideCache . Emplace(Name, FStr::SetSide(CompName, BlockActor));
		return;
	}
	BlockActor = BlockSideCache[Name];
	BlockSideCache . Emplace(Name, FStr::SetSide(CompName, BlockActor));
}

void AFoundation::Remove_Implementation(const FString& Name, const FString& CompName)
{
	if (!BlockSideCache . Contains(Name)) {
		return;
	}
	FBlockActor BlockActor = FStr::SetSide(CompName, BlockSideCache[Name], true);
	if (!BlockActor . Right && !BlockActor . Low && !BlockActor . Left && !BlockActor . Up) {
		BlockSideCache . Remove(Name);
	} else {
		BlockSideCache . Emplace(Name, BlockActor);
	}
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
	FCollisionQueryParams Params;
	Params . AddIgnoredActor(this);
	Params . bTraceComplex = false;
	bool CenterIsHit = GetWorld() -> LineTraceSingleByObjectType(
		Center, CenterStart,
		Direction * FStatic::TenThousand + CenterStart,
		ECC_WorldStatic, Params
	);
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

void AFoundation::RangeDetectRay()
{
	if (!Local -> UID || GetNetMode() == NM_DedicatedServer) {
		return;
	}
	FHitResult Center;
	FVector CenterStart = GetActorLocation();
	FCollisionQueryParams Params;
	Params . AddIgnoredActor(this);
	Params . bTraceComplex = false;
	bool bIsHit = GetWorld() -> LineTraceSingleByProfile(
		Center, CenterStart,
		-GetActorUpVector() * FStatic::Hundred + CenterStart,
		*(FStatic::Foundation + FStatic::Range), Params
	);
	if (!bIsHit) {
		EnemyRange = false;
		return;
	}
	if (!FStr::IsContain(Center . GetComponent() -> GetName(), FStatic::Range)) {
		EnemyRange = false;
		return;
	}
	if (!Center . GetActor() -> Tags . Num()) {
		EnemyRange = false;
		return;
	}
	FString Index = Center . GetActor() -> Tags[FStatic::Zero] . ToString();
	JudgeEnemyRange(Index, Local -> CID, Local -> UID);
}

void AFoundation::JudgeEnemyRange_Implementation(const FString& Index, int CID, int UID)
{
	if (Global -> Buildings[Index] . CID != CID && CID != FStatic::Zero) {
		EnemyRange = true;
		return;
	}
	if (Global -> Buildings[Index] . CID == FStatic::Zero && Global -> Buildings[Index] . UID != UID) {
		EnemyRange = true;
		return;
	}
	EnemyRange = false;
}
