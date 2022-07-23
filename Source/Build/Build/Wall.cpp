#include "Wall.h"

AWall::AWall()
{
	PrimaryActorTick . bCanEverTick = true;

	HalfYZ = FStatic::TwoHundred;
	HalfX = FStatic::Five;
	XYZScale = FStatic::Fifty;
	OverlapCount = FStatic::Zero;
	AttachCount = FStatic::Zero;

	bReplicates = true;
	bNetLoadOnClient = true;
	bAlwaysRelevant = true;
	// NetCullDistanceSquared = 1000;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(*FStatic::BoxComponent);
	BoxComponent -> InitBoxExtent(FVector(FStatic::Five, FStatic::HundredAndNinety, FStatic::HundredAndNinety));
	SetRootComponent(BoxComponent);

	FrontBoxComponent = CreateDefaultSubobject<UBoxComponent>(*FStatic::Front);
	FrontBoxComponent -> InitBoxExtent(FVector(FStatic::Ten, FStatic::Hundred, FStatic::Hundred));
	FrontBoxComponent -> SetRelativeLocation(FVector(-FStatic::Ten, FStatic::Zero, FStatic::Zero));
	FrontBoxComponent -> SetupAttachment(RootComponent);

	BackBoxComponent = CreateDefaultSubobject<UBoxComponent>(*FStatic::Back);
	BackBoxComponent -> InitBoxExtent(FVector(FStatic::Ten, FStatic::Hundred, FStatic::Hundred));
	BackBoxComponent -> SetRelativeLocation(FVector(FStatic::Ten, FStatic::Zero, FStatic::Zero));
	BackBoxComponent -> SetupAttachment(RootComponent);

	LowSideBoxComponent = CreateDefaultSubobject<UBoxComponent>(*FStatic::Low);
	LowSideBoxComponent -> InitBoxExtent(FVector(FStatic::Six, FStatic::HundredAndNinety, FStatic::Hundred));
	LowSideBoxComponent -> SetRelativeLocation(FVector(FStatic::Zero, FStatic::Zero, -FStatic::Hundred));
	LowSideBoxComponent -> SetupAttachment(RootComponent);

	UpSideBoxComponent = CreateDefaultSubobject<UBoxComponent>(*FStatic::Up);
	UpSideBoxComponent -> InitBoxExtent(FVector(FStatic::Six, FStatic::HundredAndNinety, FStatic::Hundred));
	UpSideBoxComponent -> SetRelativeLocation(FVector(FStatic::Zero, FStatic::Zero, FStatic::Hundred));
	UpSideBoxComponent -> SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> Floor(*FStatic::CubeStaticMesh);
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(*FStatic::StaticMeshComponent);
	StaticMeshComponent -> SetupAttachment(RootComponent);
	StaticMeshComponent -> SetStaticMesh(Floor . Object);
	StaticMeshComponent -> SetRelativeScale3D(FVector(HalfX / XYZScale, HalfYZ / XYZScale, HalfYZ / XYZScale));
	StaticMeshComponent -> SetRelativeLocation(FVector(FStatic::Zero, FStatic::Zero, -HalfYZ));
	StaticMeshComponent -> SetOnlyOwnerSee(true);
}

void AWall::BeginPlay()
{
	Super::BeginPlay();
	SetReplicateMovement(true);
	Local = GetGameInstance() -> GetSubsystem<ULocal>();
	Global = Global = GetWorld() -> GetGameState<AGlobal>();
	SetMaterial(FStatic::BlurMaterial);
	SetCollision(ECollisionEnabled::QueryOnly);
	BoxComponent -> OnComponentBeginOverlap . AddDynamic(this, &ThisClass::OnOverlapBegin);
	BoxComponent -> OnComponentEndOverlap . AddDynamic(this, &ThisClass::OnOverlapEnd);

	FrontBoxComponent -> OnComponentBeginOverlap . AddDynamic(this, &ThisClass::FrontOverlapBegin);
	FrontBoxComponent -> OnComponentEndOverlap . AddDynamic(this, &ThisClass::FrontOverlapEnd);
	BackBoxComponent -> OnComponentBeginOverlap . AddDynamic(this, &ThisClass::BackOverlapBegin);
	BackBoxComponent -> OnComponentEndOverlap . AddDynamic(this, &ThisClass::BackOverlapEnd);

	LowSideBoxComponent -> OnComponentBeginOverlap . AddDynamic(this, &ThisClass::LowOverlapBegin);
	LowSideBoxComponent -> OnComponentEndOverlap . AddDynamic(this, &ThisClass::LowOverlapEnd);
	UpSideBoxComponent -> OnComponentBeginOverlap . AddDynamic(this, &ThisClass::UpOverlapBegin);
	UpSideBoxComponent -> OnComponentEndOverlap . AddDynamic(this, &ThisClass::UpOverlapEnd);
}

void AWall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!IsSet) {
		this -> LandHeight();
		if (GetNetMode() == NM_DedicatedServer || GetOwner() -> GetLocalRole() != ROLE_AutonomousProxy) {
			return;
		}
		this -> RangeDetectRay();
	} else {
		PrimaryActorTick . SetTickFunctionEnable(false);
		SetCollision(ECollisionEnabled::QueryAndPhysics);
		SetMaterial(FStatic::WoodMaterial);
		Tags . Emplace(Key);
		StaticMeshComponent -> SetOnlyOwnerSee(false);
		StaticMeshComponent -> SetMobility(EComponentMobility::Stationary);
		StaticMeshComponent -> SetCollisionObjectType(ECC_WorldStatic);
		BoxComponent -> SetCollisionObjectType(ECC_WorldStatic);
		FrontBoxComponent -> SetCollisionObjectType(ECC_WorldStatic);
		BackBoxComponent -> SetCollisionObjectType(ECC_WorldStatic);
		LowSideBoxComponent -> SetCollisionObjectType(ECC_WorldStatic);
		UpSideBoxComponent -> SetCollisionObjectType(ECC_WorldStatic);
	}
}

void AWall::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass, Key);
	DOREPLIFETIME(ThisClass, IsSet);
	DOREPLIFETIME(ThisClass, EnemyRange);
}

void AWall::SetCollision(const ECollisionEnabled::Type Type) const
{
	StaticMeshComponent -> SetCollisionEnabled(Type);
	if (Type == ECollisionEnabled::QueryOnly) {
		StaticMeshComponent -> SetCollisionResponseToAllChannels(ECR_Ignore);
	} else {
		StaticMeshComponent -> SetCollisionResponseToAllChannels(ECR_Block);
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
	if (IsSet || !FStr::IsOverlapContain(OtherComp -> GetName())) {
		return;
	}
	OverlapCount += FStatic::One;
	IsBlock = true;
}

void AWall::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
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

void AWall::FrontOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
                              class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                              const FHitResult& SweepResult)
{
	if (FStr::IsContain(OtherComp -> GetName(), FStatic::Needle)) {
		Front = true;
	}
}

void AWall::FrontOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
                            class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (FStr::IsContain(OtherComp -> GetName(), FStatic::Needle)) {
		Front = false;
	}
}

void AWall::BackOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
                             class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                             const FHitResult& SweepResult)
{
	if (FStr::IsContain(OtherComp -> GetName(), FStatic::Needle)) {
		Back = true;
	}
}

void AWall::BackOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
                           class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (FStr::IsContain(OtherComp -> GetName(), FStatic::Needle)) {
		Back = false;
	}
}

void AWall::LowOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
                            class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                            const FHitResult& SweepResult)
{
	if (FStr::IsContain(OtherComp -> GetName(), FStatic::Front) || FStr::IsContain(
		OtherComp -> GetName(), FStatic::Back)) {
		return;
	}
	FString Name = OtherActor -> GetName();
	const FString CompName = OtherComp -> GetName();
	if (IsSet || !FStr::IsBuildContain(Name) || Name == GetName() || !FStr::IsSideContain(CompName) || !OtherActor -> Tags . Num()) {
		return;
	}
	Name = OtherActor -> Tags[FStatic::Zero] . ToString();
	if (Save(Name, CompName)) {
		this -> SetOwnSide(Name, CompName);
	}
}

void AWall::LowOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
                          class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (FStr::IsContain(OtherComp -> GetName(), FStatic::Front) || FStr::IsContain(
		OtherComp -> GetName(), FStatic::Back)) {
		return;
	}
	FString Name = OtherActor -> GetName();
	const FString CompName = OtherComp -> GetName();
	if (IsSet || !FStr::IsBuildContain(Name) || Name == GetName() || !FStr::IsSideContain(CompName) || !OtherActor -> Tags . Num()) {
		return;
	}
	Name = OtherActor -> Tags[FStatic::Zero] . ToString();
	if (Remove(Name, CompName)) {
		this -> SetOwnSide(Name, CompName, true);
	}
}

void AWall::UpOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
                           class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                           const FHitResult& SweepResult)
{
	if (FStr::IsContain(OtherActor -> GetName(), FStatic::Foundation) ||
		FStr::IsContain(OtherComp -> GetName(), FStatic::Front) || FStr::IsContain(
			OtherComp -> GetName(), FStatic::Back)) {
		return;
	}
	FString Name = OtherActor -> GetName();
	const FString CompName = OtherComp -> GetName();
	if (IsSet || !FStr::IsBuildContain(Name) || Name == GetName() || !FStr::IsSideContain(CompName) || !OtherActor -> Tags . Num()) {
		return;
	}
	Name = OtherActor -> Tags[FStatic::Zero] . ToString();
	if (Save(Name, CompName)) {
		this -> SetOwnSide(Name, CompName);
	}
}

void AWall::UpOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
                         class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (FStr::IsContain(OtherActor -> GetName(), FStatic::Foundation) ||
		FStr::IsContain(OtherComp -> GetName(), FStatic::Front) || FStr::IsContain(
			OtherComp -> GetName(), FStatic::Back)) {
		return;
	}
	FString Name = OtherActor -> GetName();
	const FString CompName = OtherComp -> GetName();
	if (IsSet || !FStr::IsBuildContain(Name) || Name == GetName() || !FStr::IsSideContain(CompName) || !OtherActor -> Tags . Num()) {
		return;
	}
	Name = OtherActor -> Tags[FStatic::Zero] . ToString();
	if (Remove(Name, CompName)) {
		this -> SetOwnSide(Name, CompName, true);
	}
}

bool AWall::Save(FString Name, FString CompName)
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

bool AWall::Remove(FString Name, FString CompName)
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

void AWall::LandHeight()
{
	const float UpZ = this -> RayMax(true) + FStatic::Eighty;
	const float DownZ = this -> RayMax(false) + FStatic::Eighty;
	if (FStr::IsContain(UpRayName, FStatic::Foundation)) {
		LandZ = UpZ;
		return;
	}
	if (FStr::IsContain(DownRayName, FStatic::Foundation)) {
		LandZ = DownZ;
		return;
	}
	if (FStr::IsContain(DownRayName, FStatic::Floor)) {
		LandZ = DownZ;
		return;
	}
	LandZ = FLib::Max(UpZ, DownZ);
}

float AWall::RayMax(bool IsUp)
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

void AWall::SetOwnSide(const FString Name, const FString Side, const bool IsRemove)
{
	if (FStr::IsContain(Name, FStatic::Foundation) || FStr::IsContain(Name, FStatic::Floor)) {
		// if (!IsRemove && !FStr::IsContain(Side, FStatic::Down)) {
		if (!IsRemove) {
			Low = true;
		} else {
			Low = false;
		}
	} else {
		if (!IsRemove) {
			if (Side == FStatic::Low) {
				Up = true;
			} else if (Side == FStatic::Up) {
				Low = true;
			}
		} else {
			if (Side == FStatic::Low) {
				Up = false;
			} else if (Side == FStatic::Up) {
				Low = false;
			}
		}
	}
}

void AWall::RangeDetectRay()
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

void AWall::JudgeEnemyRange_Implementation(const FString& Index, int CID, int UID)
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
