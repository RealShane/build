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
}

void AWall::BeginPlay()
{
	Super::BeginPlay();
	this -> SetMaterial(FStatic::BlurMaterial);
	BoxComponent -> OnComponentBeginOverlap . AddDynamic(this, &AWall::OnOverlapBegin);
	BoxComponent -> OnComponentEndOverlap . AddDynamic(this, &AWall::OnOverlapEnd);

	FrontBoxComponent -> OnComponentBeginOverlap . AddDynamic(this, &AWall::FrontOverlapBegin);
	FrontBoxComponent -> OnComponentEndOverlap . AddDynamic(this, &AWall::FrontOverlapEnd);
	BackBoxComponent -> OnComponentBeginOverlap . AddDynamic(this, &AWall::BackOverlapBegin);
	BackBoxComponent -> OnComponentEndOverlap . AddDynamic(this, &AWall::BackOverlapEnd);

	LowSideBoxComponent -> OnComponentBeginOverlap . AddDynamic(this, &AWall::LowOverlapBegin);
	LowSideBoxComponent -> OnComponentEndOverlap . AddDynamic(this, &AWall::LowOverlapEnd);
	UpSideBoxComponent -> OnComponentBeginOverlap . AddDynamic(this, &AWall::UpOverlapBegin);
	UpSideBoxComponent -> OnComponentEndOverlap . AddDynamic(this, &AWall::UpOverlapEnd);
}

void AWall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!IsSet) {
		this -> LandHeight();
	}
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
	if (Save(OtherActor -> GetName(), OtherComp -> GetName())) {
		this -> SetOwnSide(OtherActor -> GetName(), OtherComp -> GetName());
	}
}

void AWall::LowOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
                          class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (FStr::IsContain(OtherComp -> GetName(), FStatic::Front) || FStr::IsContain(
		OtherComp -> GetName(), FStatic::Back)) {
		return;
	}
	if (Remove(OtherActor -> GetName(), OtherComp -> GetName())) {
		this -> SetOwnSide(OtherActor -> GetName(), OtherComp -> GetName(), true);
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
	if (Save(OtherActor -> GetName(), OtherComp -> GetName())) {
		this -> SetOwnSide(OtherActor -> GetName(), OtherComp -> GetName());
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
	if (Remove(OtherActor -> GetName(), OtherComp -> GetName())) {
		this -> SetOwnSide(OtherActor -> GetName(), OtherComp -> GetName(), true);
	}
}

bool AWall::Save(FString Name, FString CompName)
{
	if (IsSet || !FStr::IsBuildContain(Name) || Name == GetName() || !FStr::IsSideContain(CompName)) {
		return false;
	}
	AttachCount += FStatic::One;
	IsAttach = true;
	if (!BlockCountCache . Contains(Name + CompName)) {
		BlockCountCache . Emplace(Name + CompName, FStatic::One);
	} else {
		int BlockCount = BlockCountCache[Name + CompName];
		BlockCount += FStatic::One;
		BlockCountCache . Emplace(Name + CompName, BlockCount);
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

bool AWall::Remove(FString Name, FString CompName)
{
	if (IsSet || !FStr::IsBuildContain(Name) || Name == GetName() || !FStr::IsSideContain(CompName)) {
		return false;
	}
	AttachCount -= FStatic::One;
	if (AttachCount <= FStatic::Zero) {
		IsAttach = false;
	}
	if (BlockCountCache . Contains(Name + CompName)) {
		int BlockCount = BlockCountCache[Name + CompName];
		BlockCount -= FStatic::One;
		if (BlockCount <= FStatic::Zero) {
			BlockCountCache . Remove(Name + CompName);
		} else {
			BlockCountCache . Emplace(Name + CompName, BlockCount);
			return false;
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
		if (!IsRemove && !FStr::IsContain(Side, FStatic::Down)) {
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
