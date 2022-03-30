// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Build/Character/MainController.h"
#include "Build/Character/MainCharacter.h"
#include "WorldGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class BUILD_API AWorldGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	AWorldGameModeBase();
	
};
