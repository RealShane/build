#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Build/Character/MainCharacter.h"
#include "Foundation.h"
#include "Wall.h"
#include "Floor.h"
#include "Build/Lib/Static.h"
#include "Build/Lib/Lib.h"
#include "Struct/Buildings.h"
#include "BuildSystem.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BUILD_API UBuildSystem : public UActorComponent
{
	GENERATED_BODY()

public:
	UBuildSystem();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	AMainCharacter* Player = nullptr;

	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	float BuildDistance;

	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	FVector BuildLocation;

	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	AFoundation* FoundationBase = nullptr;

	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	AWall* WallBase = nullptr;

	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	AFloor* FloorBase = nullptr;

	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	FString BuildType;

	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	TMap<FString, FBuildings> Buildings;

	void SetPlayer(AMainCharacter* Value);
	void SetBuild(FString Type);
	void UnSetBuild();
	void BlurAttach();
	bool Building();
	bool IsBuildMode() const;


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

	/**
	 * 天花板
	 */
	void Floor();
	void FloorBlurAttach();
	bool FloorBuild();
};
