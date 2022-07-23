#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Runtime/Online/HTTP/Public/Http.h"
#include "Interfaces/IHttpResponse.h"
#include "Build/UI/Notify/PopLayer.h"
#include "Build/GameMode/Local.h"
#include "Build/UI/UIFacade.h"
#include "Static.h"
#include "Lang.h"
#include "Lib.h"
#include "Request.generated.h"

DECLARE_MULTICAST_DELEGATE(FWidgetSwitch);

UCLASS()
class BUILD_API URequest : public UActorComponent
{
	GENERATED_BODY()
	
public:

	URequest();

	virtual void BeginPlay() override;

	FWidgetSwitch WidgetSwitch;

	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	UUIFacade* UI;

	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	UPopLayer* PopLayer;

	FLang Lang;

	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	ULocal* Local;
	
	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	UWorld* World;
	
	void SetLocal(ULocal* Instance);

	void Login(FString String);
	void LoginReceive(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void Server();
	void ServerReceive(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
};
