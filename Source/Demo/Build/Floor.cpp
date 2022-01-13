// Fill out your copyright notice in the Description page of Project Settings.


#include "Floor.h"

#include "Demo/Lib.h"

// Sets default values
AFloor::AFloor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Floor(TEXT("StaticMesh'/Game/StarterContent/Architecture/Floor_400x400.Floor_400x400'"));
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent -> SetupAttachment(RootComponent);
	StaticMeshComponent -> SetStaticMesh(Floor.Object);
}

// Called when the game starts or when spawned
void AFloor::BeginPlay()
{
	Super::BeginPlay();
	this -> SetMaterial(TEXT("Material'/Game/StarterContent/Materials/M_Tech_Hex_Tile.M_Tech_Hex_Tile'"));
}

// Called every frame
void AFloor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AFloor::SetCollision(ECollisionEnabled::Type Type)
{
	StaticMeshComponent -> SetCollisionEnabled(Type);
}

void AFloor::SetMaterial(FString Value)
{
	UMaterialInterface* Material = LoadObject<UMaterialInterface>(NULL, *Value);
	StaticMeshComponent -> SetMaterial(0, Material);
}
