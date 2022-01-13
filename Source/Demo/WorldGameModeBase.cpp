// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldGameModeBase.h"

#include "Lib.h"

AWorldGameModeBase::AWorldGameModeBase()
{
	PlayerControllerClass = APersonController::StaticClass();
	DefaultPawnClass = AMainCharacter::StaticClass();
}

void AWorldGameModeBase::BeginPlay()
{
	Super::BeginPlay();
}
