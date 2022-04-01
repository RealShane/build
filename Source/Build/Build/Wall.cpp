// Fill out your copyright notice in the Description page of Project Settings.


#include "Wall.h"

// Sets default values
AWall::AWall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent -> InitBoxExtent(FVector(5, 190, 190));
	SetRootComponent(BoxComponent);
	
	RightSideBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Right"));
	RightSideBoxComponent -> InitBoxExtent(FVector(5, 10, 100));
	RightSideBoxComponent -> SetRelativeLocation(FVector(0, 200, 0));
	RightSideBoxComponent -> SetupAttachment(RootComponent);

	LowSideBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Low"));
	LowSideBoxComponent -> InitBoxExtent(FVector(5, 100, 100));
	LowSideBoxComponent -> SetRelativeLocation(FVector(0, 0, -110));
	LowSideBoxComponent -> SetupAttachment(RootComponent);
	
	LeftSideBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Left"));
	LeftSideBoxComponent -> InitBoxExtent(FVector(5, 10, 100));
	LeftSideBoxComponent -> SetRelativeLocation(FVector(0, -200, 0));
	LeftSideBoxComponent -> SetupAttachment(RootComponent);
	
	UpSideBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Up"));
	UpSideBoxComponent -> InitBoxExtent(FVector(5, 100, 10));
	UpSideBoxComponent -> SetRelativeLocation(FVector(0, 0, 200));
	UpSideBoxComponent -> SetupAttachment(RootComponent);


	static ConstructorHelpers::FObjectFinder<UStaticMesh> Floor(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube'"));
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent -> SetupAttachment(RootComponent);
	StaticMeshComponent -> SetStaticMesh(Floor.Object);
	StaticMeshComponent -> SetRelativeScale3D(FVector(HalfX / XYZScale, HalfYZ / XYZScale, HalfYZ / XYZScale));
	StaticMeshComponent -> SetRelativeLocation(FVector(0, 0, -HalfYZ));
	
}

// Called when the game starts or when spawned
void AWall::BeginPlay()
{
	Super::BeginPlay();
	this -> SetMaterial(TEXT("Material'/Game/StarterContent/Materials/M_ColorGrid_LowSpec.M_ColorGrid_LowSpec'"));
	BoxComponent -> OnComponentBeginOverlap.AddDynamic(this, &AWall::OnOverlapBegin);
	BoxComponent -> OnComponentEndOverlap.AddDynamic(this, &AWall::OnOverlapEnd);

	RightSideBoxComponent -> OnComponentBeginOverlap.AddDynamic(this, &AWall::RightOverlapBegin);
	RightSideBoxComponent -> OnComponentEndOverlap.AddDynamic(this, &AWall::RightOverlapEnd);
	LowSideBoxComponent -> OnComponentBeginOverlap.AddDynamic(this, &AWall::LowOverlapBegin);
	LowSideBoxComponent -> OnComponentEndOverlap.AddDynamic(this, &AWall::LowOverlapEnd);
	LeftSideBoxComponent -> OnComponentBeginOverlap.AddDynamic(this, &AWall::LeftOverlapBegin);
	LeftSideBoxComponent -> OnComponentEndOverlap.AddDynamic(this, &AWall::LeftOverlapEnd);
	UpSideBoxComponent -> OnComponentBeginOverlap.AddDynamic(this, &AWall::UpOverlapBegin);
	UpSideBoxComponent -> OnComponentEndOverlap.AddDynamic(this, &AWall::UpOverlapEnd);
}

// Called every frame
void AWall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!IsSet) {
		// Lib::echo("bool : " + this -> GetName() + "---" + FString::SanitizeFloat(IsBlock));
	}
}


void AWall::SetCollision(ECollisionEnabled::Type Type)
{
	StaticMeshComponent -> SetCollisionEnabled(Type);
	if (Type == ECollisionEnabled::QueryOnly) {
		StaticMeshComponent -> SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	}else {
		StaticMeshComponent -> SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	}
}

void AWall::SetMaterial(FString Value)
{
	UMaterialInterface* Material = LoadObject<UMaterialInterface>(nullptr, *Value);
	StaticMeshComponent -> SetMaterial(0, Material);
}

void AWall::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Lib::echo("begin : " + OtherActor -> GetName() + "-" + OtherComp -> GetName());
	if (!IsSet && Str::IsOverlapContain(OtherComp -> GetName())) {
		OverlapCount += 1;
		IsBlock = true;
	}
}
 
void AWall::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// Lib::echo("end : " + OtherActor -> GetName() + "-" + OtherComp -> GetName());
	if (!IsSet && Str::IsOverlapContain(OtherComp -> GetName())) {
		OverlapCount -= 1;
		if (OverlapCount <= 0) {
			IsBlock = false;
		}
	}
}

void AWall::RightOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Str::IsContain(OtherActor -> GetName(), "Wall")) {
		if (Save(OtherActor -> GetName(), OtherComp -> GetName())) {
			Right = true;
		}
	}
}
 
void AWall::RightOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (Str::IsContain(OtherActor -> GetName(), "Wall")) {
		if (Remove(OtherActor -> GetName(), OtherComp -> GetName())) {
			Right = false;
		}
	}
}

void AWall::LowOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Save(OtherActor -> GetName(), OtherComp -> GetName())) {
		Low = true;
	}
}
 
void AWall::LowOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (Remove(OtherActor -> GetName(), OtherComp -> GetName())) {
		Low = false;
	}
}

void AWall::LeftOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Str::IsContain(OtherActor -> GetName(), "Wall")) {
		if (Save(OtherActor -> GetName(), OtherComp -> GetName())) {
			Left = true;
		}
	}
}
 
void AWall::LeftOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (Str::IsContain(OtherActor -> GetName(), "Wall")) {
		if (Remove(OtherActor -> GetName(), OtherComp -> GetName())) {
			Left = false;
		}
	}
}

void AWall::UpOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Str::IsContain(OtherActor -> GetName(), "Wall")) {
		if (Save(OtherActor -> GetName(), OtherComp -> GetName())) {
			Up = true;
		}
	}
}
 
void AWall::UpOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (Str::IsContain(OtherActor -> GetName(), "Wall")) {
		if (Remove(OtherActor -> GetName(), OtherComp -> GetName())) {
			Up = false;
		}
	}
}

bool AWall::Save(FString Name, FString CompName)
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

bool AWall::Remove(FString Name, FString CompName)
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