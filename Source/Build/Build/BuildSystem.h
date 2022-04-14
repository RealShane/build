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
	UBuildSystem();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

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
	TMap<FString, FBuildCache> Saving;

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
};
