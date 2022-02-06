// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Demo/Character/MainCharacter.h"
#include "Floor.h"
#include "BuildSystem.generated.h"

USTRUCT(BlueprintType)
struct FBuildCache
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HealthPoints;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Token;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UObject* Building;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Location;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator Rotation;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Right = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Low = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Left = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Up = false;
	
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DEMO_API UBuildSystem : public UActorComponent
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
	TArray<FBuildCache> Saving;

	void SetPlayer(AMainCharacter* Value);
	void SetBuild();
	void UnSetBuild();
	void BlurAttach();
	bool Building();
	bool IsCollision();
};
