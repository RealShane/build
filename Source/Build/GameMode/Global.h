#pragma once

#include "CoreMinimal.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/GameStateBase.h"
#include "Build/Build/Struct/BlockActor.h"
#include "Build/Build/Struct/Buildings.h"
#include "Struct/User.h"
#include "Global.generated.h"

UCLASS()
class BUILD_API AGlobal : public AGameStateBase
{
	GENERATED_BODY()

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/**
	 * 共用同步数据存储
	 */
	
	// 玩家
	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	TMap<FString, FUser> Joint;
	
	// 建筑数据
	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	TMap<FString, FBuildings> Buildings;
	
};
