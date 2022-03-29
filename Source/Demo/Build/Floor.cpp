// Fill out your copyright notice in the Description page of Project Settings.


#include "Floor.h"

#include "Demo/Lib/Lib.h"
#include "Demo/Lib/Str.h"

// Sets default values
AFloor::AFloor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent -> InitBoxExtent(FVector(HalfXY, HalfXY, HalfZ));
	SetRootComponent(BoxComponent);
	
	RightSideBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Right"));
	RightSideBoxComponent -> InitBoxExtent(FVector(100, 10, 30));
	RightSideBoxComponent -> SetRelativeLocation(FVector(0, 200, 0));
	RightSideBoxComponent -> SetupAttachment(RootComponent);

	LowSideBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Low"));
	LowSideBoxComponent -> InitBoxExtent(FVector(10, 100, 30));
	LowSideBoxComponent -> SetRelativeLocation(FVector(-200, 0, 0));
	LowSideBoxComponent -> SetupAttachment(RootComponent);

	LeftSideBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Left"));
	LeftSideBoxComponent -> InitBoxExtent(FVector(100, 10, 30));
	LeftSideBoxComponent -> SetRelativeLocation(FVector(0, -200, 0));
	LeftSideBoxComponent -> SetupAttachment(RootComponent);

	UpSideBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Up"));
	UpSideBoxComponent -> InitBoxExtent(FVector(10, 100, 30));
	UpSideBoxComponent -> SetRelativeLocation(FVector(200, 0, 0));
	UpSideBoxComponent -> SetupAttachment(RootComponent);
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Floor(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube'"));
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent -> SetupAttachment(RootComponent);
	StaticMeshComponent -> SetStaticMesh(Floor.Object);
	StaticMeshComponent -> SetRelativeScale3D(FVector(HalfXY / XYZScale, HalfXY / XYZScale, HalfZ / XYZScale));
	StaticMeshComponent -> SetRelativeLocation(FVector(0, 0, -HalfZ));
}

// Called when the game starts or when spawned
void AFloor::BeginPlay()
{
	Super::BeginPlay();
	// Material'/Game/StarterContent/Materials/M_Tech_Hex_Tile.M_Tech_Hex_Tile'
	this -> SetMaterial(TEXT("Material'/Game/StarterContent/Materials/M_ColorGrid_LowSpec.M_ColorGrid_LowSpec'"));
	BoxComponent -> OnComponentBeginOverlap.AddDynamic(this, &AFloor::OnOverlapBegin);
	BoxComponent -> OnComponentEndOverlap.AddDynamic(this, &AFloor::OnOverlapEnd);

	RightSideBoxComponent -> OnComponentBeginOverlap.AddDynamic(this, &AFloor::RightOverlapBegin);
	RightSideBoxComponent -> OnComponentEndOverlap.AddDynamic(this, &AFloor::RightOverlapEnd);
	LowSideBoxComponent -> OnComponentBeginOverlap.AddDynamic(this, &AFloor::LowOverlapBegin);
	LowSideBoxComponent -> OnComponentEndOverlap.AddDynamic(this, &AFloor::LowOverlapEnd);
	LeftSideBoxComponent -> OnComponentBeginOverlap.AddDynamic(this, &AFloor::LeftOverlapBegin);
	LeftSideBoxComponent -> OnComponentEndOverlap.AddDynamic(this, &AFloor::LeftOverlapEnd);
	UpSideBoxComponent -> OnComponentBeginOverlap.AddDynamic(this, &AFloor::UpOverlapBegin);
	UpSideBoxComponent -> OnComponentEndOverlap.AddDynamic(this, &AFloor::UpOverlapEnd);
}

// Called every frame
void AFloor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Lib::echo("Block bool : " + FString::SanitizeFloat(IsBlock));
}

void AFloor::SetCollision(ECollisionEnabled::Type Type)
{
	StaticMeshComponent -> SetCollisionEnabled(Type);
	if (Type == ECollisionEnabled::QueryOnly) {
		StaticMeshComponent -> SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	}else {
		StaticMeshComponent -> SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	}
}

void AFloor::SetMaterial(FString Value)
{
	UMaterialInterface* Material = LoadObject<UMaterialInterface>(NULL, *Value);
	StaticMeshComponent -> SetMaterial(0, Material);
}

void AFloor::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!IsSet) {
		if (OtherComp -> GetName() == "BoxComponent") {
			//TODO 这里有bug 碰撞体积来回切换01 无法判定是否为重叠 不能放置
			OverlapActorName = OtherActor -> GetName();
			IsBlock = true;
			Lib::echo("Block begin : " + this -> GetName() + " --- " + OtherActor -> GetName() + " --- " + OtherComp -> GetName());
		}
	}
}
 
void AFloor::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!IsSet) {
		if (OtherComp -> GetName() == "BoxComponent" && OtherActor -> GetName() == OverlapActorName) {
			OverlapActorName = nullptr;
			IsBlock = false;
			Lib::echo("Block end : " + this -> GetName() + " --- " + OtherActor -> GetName() + " --- " + OtherComp -> GetName());
		}
	}
}

void AFloor::RightOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Save(OtherActor -> GetName(), OtherComp -> GetName())) {
		Right = true;
	}
}
 
void AFloor::RightOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (Remove(OtherActor -> GetName(), OtherComp -> GetName())) {
		Right = false;
	}
}

void AFloor::LowOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Save(OtherActor -> GetName(), OtherComp -> GetName())) {
		Low = true;
	}
}
 
void AFloor::LowOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (Remove(OtherActor -> GetName(), OtherComp -> GetName())) {
		Low = false;
	}
}

void AFloor::LeftOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Save(OtherActor -> GetName(), OtherComp -> GetName())) {
		Left = true;
	}
}
 
void AFloor::LeftOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (Remove(OtherActor -> GetName(), OtherComp -> GetName())) {
		Left = false;
	}
}

void AFloor::UpOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Save(OtherActor -> GetName(), OtherComp -> GetName())) {
		Up = true;
	}
}
 
void AFloor::UpOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (Remove(OtherActor -> GetName(), OtherComp -> GetName())) {
		Up = false;
	}
}

bool AFloor::Save(FString Name, FString CompName)
{
	if (!IsSet && Str::IsBuildContain(Name) && Name != GetName() && Str::IsSideContain(CompName)) {
		IsAttach = true;
		BlockActorName = Name;
		BlockActorSide = CompName;
		FBlockActor BlockActor;
		BlockActor.Name = Name;
		BlockActor.Side = CompName;
		BlockSideCache.Emplace(BlockActor);
		return true;
	}
	return false;
}

bool AFloor::Remove(FString Name, FString CompName)
{
	if (!IsSet && !BlockSideCache.IsEmpty() && Str::IsBuildContain(Name) && Str::IsSideContain(CompName)) {
		IsAttach = false;
		BlockActorName = nullptr;
		BlockActorSide = nullptr;
		for (int i = 0; i < BlockSideCache.Num(); i++) {
			if (BlockSideCache[i].Name == Name && BlockSideCache[i].Side == CompName) {
				BlockSideCache.RemoveAt(i);
			}
		}
		return true;
	}
	return false;
}