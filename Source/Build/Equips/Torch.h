// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/PointLightComponent.h"
#include "Torch.generated.h"

UCLASS()
class BUILD_API ATorch : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATorch();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	UParticleSystemComponent* ParticleSystemComponent;

	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	UPointLightComponent* PointLightComponent;

	void LightOn();

};
