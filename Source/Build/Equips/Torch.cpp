// Fill out your copyright notice in the Description page of Project Settings.


#include "Torch.h"

#include "Components/SpotLightComponent.h"


// Sets default values
ATorch::ATorch()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Light = CreateDefaultSubobject<USpotLightComponent>(TEXT("SpotLightComponent"));
	SetRootComponent(Light);
}

// Called when the game starts or when spawned
void ATorch::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATorch::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATorch::On()
{
	FLinearColor Color = {1, 1, 1};
	Light -> SetLightColor(Color);
	Light -> SetInnerConeAngle(30);
	Light -> SetOuterConeAngle(60);
	Light -> SetIntensity(5000);
	Light -> SetSourceLength(100);
	Light -> SetSourceRadius(100);
	Light -> SetSoftSourceRadius(50);
}
