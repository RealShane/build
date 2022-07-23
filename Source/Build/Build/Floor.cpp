#include "Floor.h"

AFloor::AFloor()
{
	PrimaryActorTick . bCanEverTick = true;

	HalfXY = FStatic::TwoHundred;
	HalfZ = FStatic::Five;
	XYZScale = FStatic::Fifty;
	OverlapCount = FStatic::Zero;
	AttachCount = FStatic::Zero;

	bReplicates = true;
	bNetLoadOnClient = true;
	bAlwaysRelevant = true;
	// NetCullDistanceSquared = 1000;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(*FStatic::BoxComponent);
	BoxComponent -> InitBoxExtent(FVector(FStatic::HundredAndNinety, FStatic::HundredAndNinety, FStatic::Five));
	SetRootComponent(BoxComponent);

	RightNeedleBoxComponent = CreateDefaultSubobject<UBoxComponent>(*FStatic::RightNeedle);
	RightNeedleBoxComponent -> InitBoxExtent(FVector(FStatic::Five, FStatic::Five, FStatic::Sixty));
	RightNeedleBoxComponent ->
		SetRelativeLocation(FVector(FStatic::Zero, FStatic::HundredAndEighty, -FStatic::Hundred));
	RightNeedleBoxComponent -> SetupAttachment(RootComponent);

	LowNeedleBoxComponent = CreateDefaultSubobject<UBoxComponent>(*FStatic::LowNeedle);
	LowNeedleBoxComponent -> InitBoxExtent(FVector(FStatic::Five, FStatic::Five, FStatic::Sixty));
	LowNeedleBoxComponent -> SetRelativeLocation(FVector(-FStatic::HundredAndEighty, FStatic::Zero, -FStatic::Hundred));
	LowNeedleBoxComponent -> SetupAttachment(RootComponent);

	LeftNeedleBoxComponent = CreateDefaultSubobject<UBoxComponent>(*FStatic::LeftNeedle);
	LeftNeedleBoxComponent -> InitBoxExtent(FVector(FStatic::Five, FStatic::Five, FStatic::Sixty));
	LeftNeedleBoxComponent ->
		SetRelativeLocation(FVector(FStatic::Zero, -FStatic::HundredAndEighty, -FStatic::Hundred));
	LeftNeedleBoxComponent -> SetupAttachment(RootComponent);

	UpNeedleBoxComponent = CreateDefaultSubobject<UBoxComponent>(*FStatic::UpNeedle);
	UpNeedleBoxComponent -> InitBoxExtent(FVector(FStatic::Five, FStatic::Five, FStatic::Sixty));
	UpNeedleBoxComponent -> SetRelativeLocation(FVector(FStatic::HundredAndEighty, FStatic::Zero, -FStatic::Hundred));
	UpNeedleBoxComponent -> SetupAttachment(RootComponent);

	RightSideBoxComponent = CreateDefaultSubobject<UBoxComponent>(*FStatic::Right);
	RightSideBoxComponent -> InitBoxExtent(FVector(FStatic::Hundred, FStatic::FiftyOne, FStatic::Four));
	RightSideBoxComponent -> SetRelativeLocation(FVector(FStatic::Zero, FStatic::HundredAndFiftyOne, FStatic::Two));
	RightSideBoxComponent -> SetupAttachment(RootComponent);

	LowSideBoxComponent = CreateDefaultSubobject<UBoxComponent>(*FStatic::Low);
	LowSideBoxComponent -> InitBoxExtent(FVector(FStatic::FiftyOne, FStatic::Hundred, FStatic::Four));
	LowSideBoxComponent -> SetRelativeLocation(FVector(-FStatic::HundredAndFiftyOne, FStatic::Zero, FStatic::Two));
	LowSideBoxComponent -> SetupAttachment(RootComponent);

	LeftSideBoxComponent = CreateDefaultSubobject<UBoxComponent>(*FStatic::Left);
	LeftSideBoxComponent -> InitBoxExtent(FVector(FStatic::Hundred, FStatic::FiftyOne, FStatic::Four));
	LeftSideBoxComponent -> SetRelativeLocation(FVector(FStatic::Zero, -FStatic::HundredAndFiftyOne, FStatic::Two));
	LeftSideBoxComponent -> SetupAttachment(RootComponent);

	UpSideBoxComponent = CreateDefaultSubobject<UBoxComponent>(*FStatic::Up);
	UpSideBoxComponent -> InitBoxExtent(FVector(FStatic::FiftyOne, FStatic::Hundred, FStatic::Four));
	UpSideBoxComponent -> SetRelativeLocation(FVector(FStatic::HundredAndFiftyOne, FStatic::Zero, FStatic::Two));
	UpSideBoxComponent -> SetupAttachment(RootComponent);

	DownRightSideBoxComponent = CreateDefaultSubobject<UBoxComponent>(*FStatic::DownWallRight);
	DownRightSideBoxComponent -> InitBoxExtent(FVector(FStatic::Hundred, FStatic::FiftyOne, FStatic::Two));
	DownRightSideBoxComponent -> SetRelativeLocation(
		FVector(FStatic::Zero, FStatic::HundredAndFiftyOne, -FStatic::Four));
	DownRightSideBoxComponent -> SetupAttachment(RootComponent);

	DownLowSideBoxComponent = CreateDefaultSubobject<UBoxComponent>(*FStatic::DownWallLow);
	DownLowSideBoxComponent -> InitBoxExtent(FVector(FStatic::FiftyOne, FStatic::Hundred, FStatic::Two));
	DownLowSideBoxComponent ->
		SetRelativeLocation(FVector(-FStatic::HundredAndFiftyOne, FStatic::Zero, -FStatic::Four));
	DownLowSideBoxComponent -> SetupAttachment(RootComponent);

	DownLeftSideBoxComponent = CreateDefaultSubobject<UBoxComponent>(*FStatic::DownWallLeft);
	DownLeftSideBoxComponent -> InitBoxExtent(FVector(FStatic::Hundred, FStatic::FiftyOne, FStatic::Two));
	DownLeftSideBoxComponent -> SetRelativeLocation(
		FVector(FStatic::Zero, -FStatic::HundredAndFiftyOne, -FStatic::Four));
	DownLeftSideBoxComponent -> SetupAttachment(RootComponent);

	DownUpSideBoxComponent = CreateDefaultSubobject<UBoxComponent>(*FStatic::DownWallUp);
	DownUpSideBoxComponent -> InitBoxExtent(FVector(FStatic::FiftyOne, FStatic::Hundred, FStatic::Two));
	DownUpSideBoxComponent -> SetRelativeLocation(FVector(FStatic::HundredAndFiftyOne, FStatic::Zero, -FStatic::Four));
	DownUpSideBoxComponent -> SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> Foundation(*FStatic::CubeStaticMesh);
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(*FStatic::StaticMeshComponent);
	StaticMeshComponent -> SetupAttachment(RootComponent);
	StaticMeshComponent -> SetStaticMesh(Foundation . Object);
	StaticMeshComponent -> SetRelativeScale3D(FVector(HalfXY / XYZScale, HalfXY / XYZScale, HalfZ / XYZScale));
	StaticMeshComponent -> SetRelativeLocation(FVector(FStatic::Zero, FStatic::Zero, -HalfZ));
	StaticMeshComponent -> SetOnlyOwnerSee(true);
}

void AFloor::BeginPlay()
{
	Super::BeginPlay();
	SetReplicateMovement(true);
	Local = GetGameInstance() -> GetSubsystem<ULocal>();
	Global = Global = GetWorld() -> GetGameState<AGlobal>();
	SetMaterial(FStatic::BlurMaterial);
	SetCollision(ECollisionEnabled::QueryOnly);
	BoxComponent -> OnComponentBeginOverlap . AddDynamic(this, &ThisClass::OnOverlapBegin);
	BoxComponent -> OnComponentEndOverlap . AddDynamic(this, &ThisClass::OnOverlapEnd);

	RightNeedleBoxComponent -> OnComponentBeginOverlap . AddDynamic(this, &ThisClass::RightNeedleOverlapBegin);
	RightNeedleBoxComponent -> OnComponentEndOverlap . AddDynamic(this, &ThisClass::RightNeedleOverlapEnd);
	LowNeedleBoxComponent -> OnComponentBeginOverlap . AddDynamic(this, &ThisClass::LowNeedleOverlapBegin);
	LowNeedleBoxComponent -> OnComponentEndOverlap . AddDynamic(this, &ThisClass::LowNeedleOverlapEnd);
	LeftNeedleBoxComponent -> OnComponentBeginOverlap . AddDynamic(this, &ThisClass::LeftNeedleOverlapBegin);
	LeftNeedleBoxComponent -> OnComponentEndOverlap . AddDynamic(this, &ThisClass::LeftNeedleOverlapEnd);
	UpNeedleBoxComponent -> OnComponentBeginOverlap . AddDynamic(this, &ThisClass::UpNeedleOverlapBegin);
	UpNeedleBoxComponent -> OnComponentEndOverlap . AddDynamic(this, &ThisClass::UpNeedleOverlapEnd);

	RightSideBoxComponent -> OnComponentBeginOverlap . AddDynamic(this, &ThisClass::RightOverlapBegin);
	RightSideBoxComponent -> OnComponentEndOverlap . AddDynamic(this, &ThisClass::RightOverlapEnd);
	LowSideBoxComponent -> OnComponentBeginOverlap . AddDynamic(this, &ThisClass::LowOverlapBegin);
	LowSideBoxComponent -> OnComponentEndOverlap . AddDynamic(this, &ThisClass::LowOverlapEnd);
	LeftSideBoxComponent -> OnComponentBeginOverlap . AddDynamic(this, &ThisClass::LeftOverlapBegin);
	LeftSideBoxComponent -> OnComponentEndOverlap . AddDynamic(this, &ThisClass::LeftOverlapEnd);
	UpSideBoxComponent -> OnComponentBeginOverlap . AddDynamic(this, &ThisClass::UpOverlapBegin);
	UpSideBoxComponent -> OnComponentEndOverlap . AddDynamic(this, &ThisClass::UpOverlapEnd);

	DownRightSideBoxComponent -> OnComponentBeginOverlap . AddDynamic(this, &ThisClass::DownRightOverlapBegin);
	DownRightSideBoxComponent -> OnComponentEndOverlap . AddDynamic(this, &ThisClass::DownRightOverlapEnd);
	DownLowSideBoxComponent -> OnComponentBeginOverlap . AddDynamic(this, &ThisClass::DownLowOverlapBegin);
	DownLowSideBoxComponent -> OnComponentEndOverlap . AddDynamic(this, &ThisClass::DownLowOverlapEnd);
	DownLeftSideBoxComponent -> OnComponentBeginOverlap . AddDynamic(this, &ThisClass::DownLeftOverlapBegin);
	DownLeftSideBoxComponent -> OnComponentEndOverlap . AddDynamic(this, &ThisClass::DownLeftOverlapEnd);
	DownUpSideBoxComponent -> OnComponentBeginOverlap . AddDynamic(this, &ThisClass::DownUpOverlapBegin);
	DownUpSideBoxComponent -> OnComponentEndOverlap . AddDynamic(this, &ThisClass::DownUpOverlapEnd);
}

void AFloor::Tick(float DeltaTime)
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
		Tags . Emplace(Key);
		StaticMeshComponent -> SetOnlyOwnerSee(false);
		StaticMeshComponent -> SetMobility(EComponentMobility::Stationary);
		StaticMeshComponent -> SetCollisionObjectType(ECC_WorldStatic);
		BoxComponent -> SetCollisionObjectType(ECC_WorldStatic);
		RightSideBoxComponent -> SetCollisionObjectType(ECC_WorldStatic);
		LowSideBoxComponent -> SetCollisionObjectType(ECC_WorldStatic);
		LeftSideBoxComponent -> SetCollisionObjectType(ECC_WorldStatic);
		UpSideBoxComponent -> SetCollisionObjectType(ECC_WorldStatic);
		DownRightSideBoxComponent -> SetCollisionObjectType(ECC_WorldStatic);
		DownLowSideBoxComponent -> SetCollisionObjectType(ECC_WorldStatic);
		DownLeftSideBoxComponent -> SetCollisionObjectType(ECC_WorldStatic);
		DownUpSideBoxComponent -> SetCollisionObjectType(ECC_WorldStatic);
	}
}

void AFloor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass, Key);
	DOREPLIFETIME(ThisClass, IsSet);
	DOREPLIFETIME(ThisClass, EnemyRange);
}

void AFloor::SetCollision(const ECollisionEnabled::Type Type) const
{
	StaticMeshComponent -> SetCollisionEnabled(Type);
	if (Type == ECollisionEnabled::QueryOnly) {
		StaticMeshComponent -> SetCollisionResponseToAllChannels(ECR_Ignore);
	} else {
		StaticMeshComponent -> SetCollisionResponseToAllChannels(ECR_Block);
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
	if (IsSet || !FStr::IsOverlapContain(OtherComp -> GetName())) {
		return;
	}
	OverlapCount += FStatic::One;
	IsBlock = true;
}

void AFloor::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
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

void AFloor::RightNeedleOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
                                     class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                     const FHitResult& SweepResult)
{
	const FString CompName = OtherComp -> GetName();
	if (FStr::IsContain(CompName, FStatic::Front) || FStr::IsContain(CompName, FStatic::Back)) {
		if (!OtherActor -> Tags . Num()) {
			return;
		}
		Save(OtherActor -> Tags[FStatic::Zero] . ToString(), CompName, true);
	}
}

void AFloor::RightNeedleOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
                                   class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	const FString CompName = OtherComp -> GetName();
	if (FStr::IsContain(CompName, FStatic::Front) || FStr::IsContain(CompName, FStatic::Back)) {
		if (!OtherActor -> Tags . Num()) {
			return;
		}
		Remove(OtherActor -> Tags[FStatic::Zero] . ToString(), CompName, true);
	}
}

void AFloor::LowNeedleOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
                                   class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                   const FHitResult& SweepResult)
{
	const FString CompName = OtherComp -> GetName();
	if (FStr::IsContain(CompName, FStatic::Front) || FStr::IsContain(CompName, FStatic::Back)) {
		if (!OtherActor -> Tags . Num()) {
			return;
		}
		Save(OtherActor -> Tags[FStatic::Zero] . ToString(), CompName, true);
	}
}

void AFloor::LowNeedleOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
                                 class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	const FString CompName = OtherComp -> GetName();
	if (FStr::IsContain(CompName, FStatic::Front) || FStr::IsContain(CompName, FStatic::Back)) {
		if (!OtherActor -> Tags . Num()) {
			return;
		}
		Remove(OtherActor -> Tags[FStatic::Zero] . ToString(), CompName, true);
	}
}

void AFloor::LeftNeedleOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
                                    class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                    const FHitResult& SweepResult)
{
	const FString CompName = OtherComp -> GetName();
	if (FStr::IsContain(CompName, FStatic::Front) || FStr::IsContain(CompName, FStatic::Back)) {
		if (!OtherActor -> Tags . Num()) {
			return;
		}
		Save(OtherActor -> Tags[FStatic::Zero] . ToString(), CompName, true);
	}
}

void AFloor::LeftNeedleOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
                                  class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	const FString CompName = OtherComp -> GetName();
	if (FStr::IsContain(CompName, FStatic::Front) || FStr::IsContain(CompName, FStatic::Back)) {
		if (!OtherActor -> Tags . Num()) {
			return;
		}
		Remove(OtherActor -> Tags[FStatic::Zero] . ToString(), CompName, true);
	}
}

void AFloor::UpNeedleOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
                                  class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                  const FHitResult& SweepResult)
{
	const FString CompName = OtherComp -> GetName();
	if (FStr::IsContain(CompName, FStatic::Front) || FStr::IsContain(CompName, FStatic::Back)) {
		if (!OtherActor -> Tags . Num()) {
			return;
		}
		Save(OtherActor -> Tags[FStatic::Zero] . ToString(), CompName, true);
	}
}

void AFloor::UpNeedleOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
                                class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	const FString CompName = OtherComp -> GetName();
	if (FStr::IsContain(CompName, FStatic::Front) || FStr::IsContain(CompName, FStatic::Back)) {
		if (!OtherActor -> Tags . Num()) {
			return;
		}
		Remove(OtherActor -> Tags[FStatic::Zero] . ToString(), CompName, true);
	}
}

void AFloor::RightOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
                               class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                               const FHitResult& SweepResult)
{
	if (FStr::IsContain(OtherActor -> GetName(), FStatic::Wall) &&
		FStr::IsContain(OtherComp -> GetName(), FStatic::Low)) {
		WallRight = true;
	}
}

void AFloor::RightOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
                             class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (FStr::IsContain(OtherActor -> GetName(), FStatic::Wall) &&
		FStr::IsContain(OtherComp -> GetName(), FStatic::Low)) {
		WallRight = false;
	}
}

void AFloor::LowOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
                             class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                             const FHitResult& SweepResult)
{
	if (FStr::IsContain(OtherActor -> GetName(), FStatic::Wall) &&
		FStr::IsContain(OtherComp -> GetName(), FStatic::Low)) {
		WallLow = true;
	}
}

void AFloor::LowOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
                           class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (FStr::IsContain(OtherActor -> GetName(), FStatic::Wall) &&
		FStr::IsContain(OtherComp -> GetName(), FStatic::Low)) {
		WallLow = false;
	}
}

void AFloor::LeftOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
                              class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                              const FHitResult& SweepResult)
{
	if (FStr::IsContain(OtherActor -> GetName(), FStatic::Wall) &&
		FStr::IsContain(OtherComp -> GetName(), FStatic::Low)) {
		WallLeft = true;
	}
}

void AFloor::LeftOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
                            class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (FStr::IsContain(OtherActor -> GetName(), FStatic::Wall) &&
		FStr::IsContain(OtherComp -> GetName(), FStatic::Low)) {
		WallLeft = false;
	}
}

void AFloor::UpOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
                            class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                            const FHitResult& SweepResult)
{
	if (FStr::IsContain(OtherActor -> GetName(), FStatic::Wall) &&
		FStr::IsContain(OtherComp -> GetName(), FStatic::Low)) {
		WallUp = true;
	}
}

void AFloor::UpOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
                          class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (FStr::IsContain(OtherActor -> GetName(), FStatic::Wall) &&
		FStr::IsContain(OtherComp -> GetName(), FStatic::Low)) {
		WallUp = false;
	}
}

void AFloor::DownRightOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
                                   class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                   const FHitResult& SweepResult)
{
	if (FStr::IsContain(OtherActor -> GetName(), FStatic::Wall) &&
		FStr::IsContain(OtherComp -> GetName(), FStatic::Up)) {
		DownWallRight = true;
	}
}

void AFloor::DownRightOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
                                 class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (FStr::IsContain(OtherActor -> GetName(), FStatic::Wall) &&
		FStr::IsContain(OtherComp -> GetName(), FStatic::Up)) {
		DownWallRight = false;
	}
}

void AFloor::DownLowOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
                                 class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                 const FHitResult& SweepResult)
{
	if (FStr::IsContain(OtherActor -> GetName(), FStatic::Wall) &&
		FStr::IsContain(OtherComp -> GetName(), FStatic::Up)) {
		DownWallLow = true;
	}
}

void AFloor::DownLowOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
                               class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (FStr::IsContain(OtherActor -> GetName(), FStatic::Wall) &&
		FStr::IsContain(OtherComp -> GetName(), FStatic::Up)) {
		DownWallLow = false;
	}
}

void AFloor::DownLeftOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
                                  class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                  const FHitResult& SweepResult)
{
	if (FStr::IsContain(OtherActor -> GetName(), FStatic::Wall) &&
		FStr::IsContain(OtherComp -> GetName(), FStatic::Up)) {
		DownWallLeft = true;
	}
}

void AFloor::DownLeftOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
                                class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (FStr::IsContain(OtherActor -> GetName(), FStatic::Wall) &&
		FStr::IsContain(OtherComp -> GetName(), FStatic::Up)) {
		DownWallLeft = false;
	}
}

void AFloor::DownUpOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
                                class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                const FHitResult& SweepResult)
{
	if (FStr::IsContain(OtherActor -> GetName(), FStatic::Wall) &&
		FStr::IsContain(OtherComp -> GetName(), FStatic::Up)) {
		DownWallUp = true;
	}
}

void AFloor::DownUpOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
                              class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (FStr::IsContain(OtherActor -> GetName(), FStatic::Wall) &&
		FStr::IsContain(OtherComp -> GetName(), FStatic::Up)) {
		DownWallUp = false;
	}
}

void AFloor::RightDetectRay()
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
		if (!FStr::IsBuildContain(Name) || !FStr::IsSideContain(CompName)) {
			return;
		}
		if (FStr::IsContain(Name, FStatic::Wall) && CompName == FStatic::Up) {
			return;
		}
		Name = Hit . GetActor() -> Tags[FStatic::Zero] . ToString();
		if (Save(Name, CompName)) {
			IsAttach = true;
			RightDetectName = Name;
			RightDetectCompName = CompName;
			Right = true;
		}
		return;
	}
	if (!RightDetectName . IsEmpty()) {
		if (Remove(RightDetectName, RightDetectCompName)) {
			IsAttach = false;
			RightDetectName = nullptr;
			RightDetectCompName = nullptr;
			Right = false;
		}
	}
}

void AFloor::LowDetectRay()
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
		if (!FStr::IsBuildContain(Name) || !FStr::IsSideContain(CompName)) {
			return;
		}
		if (FStr::IsContain(Name, FStatic::Wall) && CompName == FStatic::Up) {
			return;
		}
		Name = Hit . GetActor() -> Tags[FStatic::Zero] . ToString();
		if (Save(Name, CompName)) {
			IsAttach = true;
			LowDetectName = Name;
			LowDetectCompName = CompName;
			Low = true;
		}
		return;
	}
	if (!LowDetectName . IsEmpty()) {
		if (Remove(LowDetectName, LowDetectCompName)) {
			IsAttach = false;
			LowDetectName = nullptr;
			LowDetectCompName = nullptr;
			Low = false;
		}
	}
}

void AFloor::LeftDetectRay()
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
		if (!FStr::IsBuildContain(Name) || !FStr::IsSideContain(CompName)) {
			return;
		}
		if (FStr::IsContain(Name, FStatic::Wall) && CompName == FStatic::Up) {
			return;
		}
		Name = Hit . GetActor() -> Tags[FStatic::Zero] . ToString();
		if (Save(Name, CompName)) {
			IsAttach = true;
			LeftDetectName = Name;
			LeftDetectCompName = CompName;
			Left = true;
		}
		return;
	}
	if (!LeftDetectName . IsEmpty()) {
		if (Remove(LeftDetectName, LeftDetectCompName)) {
			IsAttach = false;
			LeftDetectName = nullptr;
			LeftDetectCompName = nullptr;
			Left = false;
		}
	}
}

void AFloor::UpDetectRay()
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
		if (!FStr::IsBuildContain(Name) || !FStr::IsSideContain(CompName)) {
			return;
		}
		if (FStr::IsContain(Name, FStatic::Wall) && CompName == FStatic::Up) {
			return;
		}
		Name = Hit . GetActor() -> Tags[FStatic::Zero] . ToString();
		if (Save(Name, CompName)) {
			IsAttach = true;
			UpDetectName = Name;
			UpDetectCompName = CompName;
			Up = true;
		}
		return;
	}
	if (!UpDetectName . IsEmpty()) {
		if (Remove(UpDetectName, UpDetectCompName)) {
			IsAttach = false;
			UpDetectName = nullptr;
			UpDetectCompName = nullptr;
			Up = false;
		}
	}
}

bool AFloor::Save(FString Name, FString CompName, bool IsNeedle)
{
	if (IsNeedle) {
		AttachCount += FStatic::One;
		IsAttach = true;
	}
	FBlockActor BlockActor;
	if (!BlockSideCache . Contains(Name)) {
		BlockSideCache . Emplace(Name, FStr::SetSide(CompName, BlockActor));
		return true;
	}
	BlockActor = BlockSideCache[Name];
	BlockSideCache . Emplace(Name, FStr::SetSide(CompName, BlockActor));
	return true;
}

bool AFloor::Remove(FString Name, FString CompName, bool IsNeedle)
{
	if (IsNeedle) {
		AttachCount -= FStatic::One;
		if (AttachCount <= FStatic::Zero) {
			IsAttach = false;
		}
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

void AFloor::LandHeight()
{
	const float UpZ = this -> RayMax(true) + FStatic::Three;
	const float DownZ = this -> RayMax(false) + FStatic::Three;
	if (FStr::IsContain(UpRayName, FStatic::Foundation) || FStr::IsContain(UpRayName, FStatic::Landscape)) {
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

float AFloor::RayMax(bool IsUp)
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

void AFloor::RangeDetectRay()
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

void AFloor::JudgeEnemyRange_Implementation(const FString& Index, int CID, int UID)
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
