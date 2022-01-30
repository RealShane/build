// Fill out your copyright notice in the Description page of Project Settings.


#include "Floor.h"

#include "Demo/Lib/Lib.h"

// Sets default values
AFloor::AFloor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	RightSideBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("RightSideBoxComponent"));
	BoxComponent -> InitBoxExtent(FVector(HalfXY, HalfXY, HalfZ));
	RightSideBoxComponent -> InitBoxExtent(FVector(200, 0, 50));
	SetRootComponent(BoxComponent);
	RightSideBoxComponent -> SetupAttachment(RootComponent);
	
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
	BlockActorName = OtherActor -> GetName();
	IsBlock = true;
}
 
void AFloor::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	BlockActorName = nullptr;
	IsBlock = false;
}