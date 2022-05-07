#include "Request.h"

URequest::URequest()
{
	
}

void URequest::Send(const FString Url, const FString Type)
{
	const TSharedRef<IHttpRequest> Request = FHttpModule::Get() . CreateRequest();
	Request -> OnProcessRequestComplete() . BindUObject(this, &URequest::Receive);
	Request -> SetURL(FStatic::Domain + Url);
	Request -> SetVerb(Type);
	Request -> SetHeader(FStatic::ContentType, FStatic::ContentTypeString);
	Request -> ProcessRequest();
}

void URequest::Receive(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (!bWasSuccessful || !Response . IsValid()) {
		return;
	}
	if (!EHttpResponseCodes::IsOk(Response -> GetResponseCode())) {
		return;
	}
	FLib::Echo(Response -> GetContentAsString(), true);
}
