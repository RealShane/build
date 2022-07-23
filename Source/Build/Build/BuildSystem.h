#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Build/Character/MainCharacter.h"
#include "Build/GameMode/Global.h"
#include "Build/GameMode/Local.h"
#include "Struct/Buildings.h"
#include "Build/Lib/Static.h"
#include "Build/Lib/Lib.h"
#include "Foundation.h"
#include "Floor.h"
#include "Wall.h"
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

	FLang Lang;

	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	UUIFacade* UI;

	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	UPopLayer* PopLayer;

	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	UPopNotice* PopNotice;

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
	ULocal* Local;

	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	AGlobal* Global;

	void SetPlayer(AMainCharacter* P);
	UFUNCTION(Server, Reliable)
	void SetServerPlayer(AMainCharacter* P);
	void SetBuild(FString Type);
	void UnSetBuild();
	void BlurAttach();
	void Building();
	bool IsBuildMode() const;

	bool IsServer() const;
	bool IsClient() const;

	FString CreateBuildingKey(FString Type) const;


	/**
	 * 地基
	 */
	UFUNCTION(Server, Reliable)
	void Foundation();
	UFUNCTION(Server, Reliable)
	void FoundationBlurAttach(FHitResult RayHit, int CID, int UID);
	UFUNCTION(Server, Reliable)
	void FoundationBuild(int CID, int UID);

	/**
	 * 墙
	 */
	UFUNCTION(Server, Reliable)
	void Wall();
	UFUNCTION(Server, Reliable)
	void WallBlurAttach(FHitResult RayHit, int CID, int UID);
	UFUNCTION(Server, Reliable)
	void WallBuild(int CID, int UID);

	/**
	 * 天花板
	 */
	UFUNCTION(Server, Reliable)
	void Floor();
	UFUNCTION(Server, Reliable)
	void FloorBlurAttach(FHitResult RayHit, int CID, int UID);
	UFUNCTION(Server, Reliable)
	void FloorBuild(int CID, int UID);
	UFUNCTION(Client, Unreliable)
	void Pop(const FString& Message, const FString& Color);
};
