// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldGameModeBase.h"

AWorldGameModeBase::AWorldGameModeBase()
{
	PlayerControllerClass = AMainController::StaticClass();
	DefaultPawnClass = AMainCharacter::StaticClass();
}

void AWorldGameModeBase::BeginPlay()
{
	Super::BeginPlay();
}
