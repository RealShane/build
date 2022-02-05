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
}

// Called every frame
void AFloor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
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
		IsBlock = true;
		if (Str::IsBuildContain(OtherActor -> GetName()) && OtherActor -> GetName() != GetName()) {
			BlockActorName = OtherActor -> GetName();
			FString BlockActorComp = OtherComp -> GetName();
			if (Str::IsSideContain(BlockActorComp)) {
				FBlockActor BlockActor;
				BlockActor.Name = BlockActorName;
				BlockActor.Side = BlockActorComp;
				BlockSideCache.Emplace(BlockActor);
				if (BlockActorComp == "Right") {
					Left = true;
				}else if (BlockActorComp == "Low") {
					Up = true;
				}else if (BlockActorComp == "Left") {
					Right = true;
				}else if (BlockActorComp == "Up") {
					Low = true;
				}
				Lib::echo("insert : " + BlockActorName + " --- " + BlockActorComp);
			}
		}
	}
}
 
void AFloor::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!IsSet) {
		BlockActorName = nullptr;
		IsBlock = false;
		if (!BlockSideCache.IsEmpty()) {
			FString UnBlockActorName = OtherActor -> GetName();
			FString BlockActorComp = OtherComp -> GetName();
			if (Str::IsBuildContain(UnBlockActorName) && Str::IsSideContain(BlockActorComp)) {
				for (int i = 0; i < BlockSideCache.Num(); i++) {
					if (BlockSideCache[i].Name == UnBlockActorName && BlockSideCache[i].Side == BlockActorComp) {
						Lib::echo("remove : " + UnBlockActorName + " --- " + BlockActorComp);
						BlockSideCache.RemoveAt(i);
						if (BlockActorComp == "Right") {
							Left = true;
						}else if (BlockActorComp == "Low") {
							Up = true;
						}else if (BlockActorComp == "Left") {
							Right = true;
						}else if (BlockActorComp == "Up") {
							Low = true;
						}
					}
				}
			}
		}
	}
}