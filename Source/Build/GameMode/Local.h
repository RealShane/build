#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Local.generated.h"

UCLASS()
class BUILD_API ULocal : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	FString LoginEmail;

	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	FString LoginPassword;

	/**
	 * 选择服务器时保存的值
	 */
	TArray<TMap<FString, FString>> Server;
	FString SelectServerName;
	FString SelectServerType;
	FString SelectServerAddress;
	bool LostConnection = false;

	/**
	 * 用户信息
	 */
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int UID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int CID;

	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	FString Token;

	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	FString KeyPre;

	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	FString NickName;
};
