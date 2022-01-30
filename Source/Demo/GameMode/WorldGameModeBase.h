// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Demo/Character/MainController.h"
#include "Demo/Character/MainCharacter.h"
#include "WorldGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class DEMO_API AWorldGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	AWorldGameModeBase();
	
};
