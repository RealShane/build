#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WebSockets/Public/WebSocketsModule.h"
#include "WebSockets/Public/IWebSocket.h"
#include "Json/Public/JSON.h"
#include "Json/Public/Serialization/JsonSerializer.h"
#include "Json/Public/Serialization/JsonReader.h"
#include "Lib.h"
#include "Build/Character/CloneCharacter.h"
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
	TMap<FString, ACloneCharacter*> Players;

	void Init(UWorld* Main);
	void FreshJoin(TSharedPtr<FJsonObject> Data);
	void Move(TSharedPtr<FJsonObject> Data);

	void OnConnected();
	void OnConnectionError(const FString& Error);
	void OnMessage(const FString& Source);
	void OnMessageSent(const FString& Source);
	void OnClosed(int32 StatusCode, const FString& Reason, bool bWasClean);
	void Send(FString String);
};
