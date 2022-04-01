// Fill out your copyright notice in the Description page of Project Settings.


#include "Foundation.h"

// Sets default values
AFoundation::AFoundation()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent -> InitBoxExtent(FVector(190, 190, 30));
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
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Foundation(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube'"));
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent -> SetupAttachment(RootComponent);
	StaticMeshComponent -> SetStaticMesh(Foundation.Object);
	StaticMeshComponent -> SetRelativeScale3D(FVector(HalfXY / XYZScale, HalfXY / XYZScale, HalfZ / XYZScale));
	StaticMeshComponent -> SetRelativeLocation(FVector(0, 0, -HalfZ));
}

// Called when the game starts or when spawned
void AFoundation::BeginPlay()
{
	Super::BeginPlay();
	// Material'/Game/StarterContent/Materials/M_Tech_Hex_Tile.M_Tech_Hex_Tile'
	this -> SetMaterial(TEXT("Material'/Game/StarterContent/Materials/M_ColorGrid_LowSpec.M_ColorGrid_LowSpec'"));
	BoxComponent -> OnComponentBeginOverlap.AddDynamic(this, &AFoundation::OnOverlapBegin);
	BoxComponent -> OnComponentEndOverlap.AddDynamic(this, &AFoundation::OnOverlapEnd);

	RightSideBoxComponent -> OnComponentBeginOverlap.AddDynamic(this, &AFoundation::RightOverlapBegin);
	RightSideBoxComponent -> OnComponentEndOverlap.AddDynamic(this, &AFoundation::RightOverlapEnd);
	LowSideBoxComponent -> OnComponentBeginOverlap.AddDynamic(this, &AFoundation::LowOverlapBegin);
	LowSideBoxComponent -> OnComponentEndOverlap.AddDynamic(this, &AFoundation::LowOverlapEnd);
	LeftSideBoxComponent -> OnComponentBeginOverlap.AddDynamic(this, &AFoundation::LeftOverlapBegin);
	LeftSideBoxComponent -> OnComponentEndOverlap.AddDynamic(this, &AFoundation::LeftOverlapEnd);
	UpSideBoxComponent -> OnComponentBeginOverlap.AddDynamic(this, &AFoundation::UpOverlapBegin);
	UpSideBoxComponent -> OnComponentEndOverlap.AddDynamic(this, &AFoundation::UpOverlapEnd);
}

// Called every frame
void AFoundation::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!IsSet) {
		// Lib::echo("bool : " + this -> GetName() + "---" + FString::SanitizeFloat(IsBlock));
	}
}

void AFoundation::SetCollision(ECollisionEnabled::Type Type)
{
	StaticMeshComponent -> SetCollisionEnabled(Type);
	if (Type == ECollisionEnabled::QueryOnly) {
		StaticMeshComponent -> SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	}else {
		StaticMeshComponent -> SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	}
}

void AFoundation::SetMaterial(FString Value)
{
	UMaterialInterface* Material = LoadObject<UMaterialInterface>(nullptr, *Value);
	StaticMeshComponent -> SetMaterial(0, Material);
}

void AFoundation::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!IsSet && Str::IsOverlapContain(OtherComp -> GetName())) {
		OverlapCount += 1;
		IsBlock = true;
	}
}
 
void AFoundation::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!IsSet && Str::IsOverlapContain(OtherComp -> GetName())) {
		OverlapCount -= 1;
		if (OverlapCount <= 0) {
			IsBlock = false;
		}
	}
}

void AFoundation::RightOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Save(OtherActor -> GetName(), OtherComp -> GetName())) {
		Right = true;
	}
}
 
void AFoundation::RightOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (Remove(OtherActor -> GetName(), OtherComp -> GetName())) {
		Right = false;
	}
}

void AFoundation::LowOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Save(OtherActor -> GetName(), OtherComp -> GetName())) {
		Low = true;
	}
}
 
void AFoundation::LowOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (Remove(OtherActor -> GetName(), OtherComp -> GetName())) {
		Low = false;
	}
}

void AFoundation::LeftOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Save(OtherActor -> GetName(), OtherComp -> GetName())) {
		Left = true;
	}
}
 
void AFoundation::LeftOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (Remove(OtherActor -> GetName(), OtherComp -> GetName())) {
		Left = false;
	}
}

void AFoundation::UpOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Save(OtherActor -> GetName(), OtherComp -> GetName())) {
		Up = true;
	}
}
 
void AFoundation::UpOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (Remove(OtherActor -> GetName(), OtherComp -> GetName())) {
		Up = false;
	}
}

bool AFoundation::Save(FString Name, FString CompName)
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

bool AFoundation::Remove(FString Name, FString CompName)
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