#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Runtime/Online/HTTP/Public/Http.h"
#include "Interfaces/IHttpResponse.h"
#include "Static.h"
#include "Lib.h"
#include "Request.generated.h"

UCLASS()
class BUILD_API URequest : public UActorComponent
{
	GENERATED_BODY()
	
public:
	URequest();

	void Send(FString Url, FString Type);
	void Receive(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
};
