#pragma once

#include "CoreMinimal.h"
#include "Json/Public/Json.h"
#include "Modules/ModuleManager.h"
#include "Components/ActorComponent.h"
#include "WebSockets/Public/IWebSocket.h"
#include "WebSockets/Public/WebSocketsModule.h"
#include "Json/Public/Serialization/JsonReader.h"
#include "Json/Public/Serialization/JsonSerializer.h"
#include "Build/Character/Struct/Route.h"
#include "Build/Build/Foundation.h"
#include "Build/GameMode/Local.h"
#include "Build/Build/Floor.h"
#include "Build/Build/Wall.h"
#include "Lib.h"
#include "NetWork.generated.h"

UCLASS()
class BUILD_API UNetWork : public UActorComponent
{
	GENERATED_BODY()

public:
	UNetWork();

protected:
	virtual void BeginPlay() override;

public:
	TSharedPtr<IWebSocket> Socket;

	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	UWorld* World;

	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	ULocal* Local;

	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	FString Token;

	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	int Ping;

	void Init(UWorld* Main);

	void Pong();
	void GuestQuit(FString NickName);

	void OnConnected();
	void OnConnectionError(const FString& Error);
	void OnMessage(const FString& Source);
	void OnMessageSent(const FString& Source);
	void OnClosed(int32 StatusCode, const FString& Reason, bool bWasClean);
};
