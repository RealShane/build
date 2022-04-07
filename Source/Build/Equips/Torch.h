﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/SpotLight.h"
#include "GameFramework/Actor.h"
#include "Torch.generated.h"

UCLASS()
class BUILD_API ATorch : public ALight
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
	USpotLightComponent* Light;

	void On();
};