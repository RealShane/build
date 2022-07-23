#include "Request.h"

URequest::URequest()
{
	UI = CreateDefaultSubobject<UUIFacade>(*FStatic::UI);
}

void URequest::BeginPlay()
{
	Super::BeginPlay();
	PopLayer = UI -> GetInstance<UPopLayer>(*FStatic::PopLayerBP);
}

void URequest::SetLocal(ULocal* Instance)
{
	Local = Instance;
}

void URequest::Login(const FString String)
{
	const TSharedRef<IHttpRequest> Request = FHttpModule::Get() . CreateRequest();
	Request -> OnProcessRequestComplete() . BindUObject(this, &ThisClass::LoginReceive);
	Request -> SetURL(FStatic::RemoteDomain + FStatic::LoginUrl + String);
	Request -> SetVerb(FStatic::Get);
	Request -> SetHeader(FStatic::ContentType, FStatic::ContentTypeString);
	Request -> ProcessRequest();
}

void URequest::LoginReceive(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (!bWasSuccessful || !Response . IsValid()) {
		return;
	}
	if (!EHttpResponseCodes::IsOk(Response -> GetResponseCode())) {
		return;
	}
	TSharedPtr<FJsonObject> Data;
	if (!FJsonSerializer::Deserialize(TJsonReaderFactory<>::Create(Response -> GetContentAsString()), Data)) {
		return;
	}
	const int Status = Data -> GetIntegerField(FStatic::Status);
	const FString Message = Data -> GetStringField(FStatic::Message);
	if (Status == FStatic::Hundred) {
		PopLayer -> Pop(Message, FStatic::WarningFontMaterial);
		return;
	}
	const TSharedPtr<FJsonObject> Result = Data -> GetObjectField(FStatic::Result);
	Local -> UID = Result -> GetIntegerField(FStatic::UID);
	Local -> CID = Result -> GetIntegerField(FStatic::CID);
	Local -> Token = Result -> GetStringField(FStatic::Token);
	Local -> KeyPre = Result -> GetStringField(FStatic::KeyPre);
	Local -> NickName = Result -> GetStringField(FStatic::NickName);
	Server();
}

void URequest::Server()
{
	const TSharedRef<IHttpRequest> Request = FHttpModule::Get() . CreateRequest();
	Request -> OnProcessRequestComplete() . BindUObject(this, &ThisClass::ServerReceive);
	Request -> SetURL(FStatic::RemoteDomain + FStatic::ServerListUrl);
	Request -> SetVerb(FStatic::Get);
	Request -> SetHeader(FStatic::ContentType, FStatic::ContentTypeString);
	Request -> ProcessRequest();
}

void URequest::ServerReceive(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (!bWasSuccessful || !Response . IsValid()) {
		PopLayer -> Pop(Lang . Get(FStatic::ServerListReadFailText), FStatic::WarningFontMaterial);
		return;
	}
	if (!EHttpResponseCodes::IsOk(Response -> GetResponseCode())) {
		PopLayer -> Pop(Lang . Get(FStatic::ServerListReadFailText), FStatic::WarningFontMaterial);
		return;
	}
	TSharedPtr<FJsonObject> Data;
	if (!FJsonSerializer::Deserialize(TJsonReaderFactory<>::Create(Response -> GetContentAsString()), Data)) {
		PopLayer -> Pop(Lang . Get(FStatic::ServerListReadFailText), FStatic::WarningFontMaterial);
		return;
	}
	const int Status = Data -> GetIntegerField(FStatic::Status);
	const FString Message = Data -> GetStringField(FStatic::Message);
	if (Status == FStatic::Hundred) {
		PopLayer -> Pop(Message, FStatic::WarningFontMaterial);
		return;
	}
	TArray<TMap<FString, FString>> Server;
	const TArray<TSharedPtr<FJsonValue>>& Results = Data -> GetArrayField(FStatic::Result);
	for (auto Result : Results) {
		TMap<FString, FString> Temp;
		Temp . Emplace(FStatic::Name, Result -> AsObject() -> GetStringField(FStatic::Name));
		Temp . Emplace(FStatic::Type, Result -> AsObject() -> GetStringField(FStatic::Type));
		Temp . Emplace(FStatic::IP, Result -> AsObject() -> GetStringField(FStatic::IP));
		Server . Emplace(Temp);
	}
	Local -> Server = Server;
	WidgetSwitch . Broadcast();
}
