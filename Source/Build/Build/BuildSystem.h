// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Build/Character/MainCharacter.h"
#include "Foundation.h"
#include "Wall.h"
#include "Build/Lib/Lib.h"
#include "Struct/BuildCache.h"
#include "BuildSystem.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BUILD_API UBuildSystem : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UBuildSystem();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	AMainCharacter* Player;
	
	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	float BuildDistance = 500;

	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	FVector BuildLocation;

	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	UObject* BuildItem = nullptr;

	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	FString BuildType;

	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	TArray<FBuildCache> Saving;

	void SetPlayer(AMainCharacter* Value);
	void SetBuild(FString Type);
	void UnSetBuild();
	void BlurAttach();
	bool Building();
	bool IsBuildMode();


	/**
	 * 地基
	 */
	void Foundation();
	void FoundationBlurAttach();
	bool FoundationBuild();

	/**
	 * 墙
	 */
	void Wall();
	void WallBlurAttach();
	bool WallBuild();
};
