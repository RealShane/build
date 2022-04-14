#include "Wall.h"

AWall::AWall()
{
	PrimaryActorTick . bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent -> InitBoxExtent(FVector(5, 190, 190));
	SetRootComponent(BoxComponent);

	LowSideBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Low"));
	LowSideBoxComponent -> InitBoxExtent(FVector(10, 150, 85));
	LowSideBoxComponent -> SetRelativeLocation(FVector(0, 0, -125));
	LowSideBoxComponent -> SetupAttachment(RootComponent);

	UpSideBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Up"));
	UpSideBoxComponent -> InitBoxExtent(FVector(10, 150, 85));
	UpSideBoxComponent -> SetRelativeLocation(FVector(0, 0, 125));
	UpSideBoxComponent -> SetupAttachment(RootComponent);


	static ConstructorHelpers::FObjectFinder<UStaticMesh> Floor(
		TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube'"));
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent -> SetupAttachment(RootComponent);
	StaticMeshComponent -> SetStaticMesh(Floor . Object);
	StaticMeshComponent -> SetRelativeScale3D(FVector(HalfX / XYZScale, HalfYZ / XYZScale, HalfYZ / XYZScale));
	StaticMeshComponent -> SetRelativeLocation(FVector(0, 0, -HalfYZ));
}

void AWall::BeginPlay()
{
	Super::BeginPlay();
	this -> SetMaterial(TEXT("Material'/Game/StarterContent/Materials/M_ColorGrid_LowSpec.M_ColorGrid_LowSpec'"));
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
	if (!IsSet) {
		// Lib::echo("bool : " + FString::SanitizeFloat(BlockWallName.IsEmpty()));
	}
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
	StaticMeshComponent -> SetMaterial(0, Material);
}

void AWall::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
                           class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                           const FHitResult& SweepResult)
{
	if (!IsSet && FStr::IsOverlapContain(OtherComp -> GetName())) {
		OverlapCount += 1;
		IsBlock = true;
	}
}

void AWall::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
                         class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!IsSet && FStr::IsOverlapContain(OtherComp -> GetName())) {
		OverlapCount -= 1;
		if (OverlapCount <= 0) {
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
	IsAttach = true;
	if (FStr::IsContain(Name, "Foundation")) {
		BlockFoundationName = Name;
		BlockFoundationSide = CompName;
	}
	if (FStr::IsContain(Name, "Wall") && BlockWallName . IsEmpty()) {
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
	IsAttach = false;
	if (FStr::IsContain(Name, "Foundation")) {
		BlockFoundationName = nullptr;
		BlockFoundationSide = nullptr;
	}
	if (FStr::IsContain(Name, "Wall") && Name == BlockWallName) {
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
