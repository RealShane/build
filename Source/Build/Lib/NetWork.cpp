#include "NetWork.h"

UNetWork::UNetWork()
{
	if (!FModuleManager::Get() . IsModuleLoaded(*FStatic::WebSockets)) {
		FModuleManager::Get() . LoadModule(*FStatic::WebSockets);
	}
	Ping = FStatic::Zero;
}

void UNetWork::BeginPlay()
{
	Super::BeginPlay();
}

void UNetWork::Init(UWorld* Main)
{
	World = Main;
	Local = World -> GetGameInstance() -> GetSubsystem<ULocal>();
	Token = Local -> Token;
	TMap<FString, FString> Header;
	Header . Emplace(FStatic::Token, Token);
	Socket = FWebSocketsModule::Get() . CreateWebSocket(FStatic::WSRemoteURL, FStatic::WSS, Header);
	Socket -> OnConnected() . AddUObject(this, &UNetWork::OnConnected);
	Socket -> OnConnectionError() . AddUObject(this, &UNetWork::OnConnectionError);
	Socket -> OnMessage() . AddUObject(this, &UNetWork::OnMessage);
	Socket -> OnMessageSent() . AddUObject(this, &UNetWork::OnMessageSent);
	Socket -> OnClosed() . AddUObject(this, &UNetWork::OnClosed);
	Socket -> Connect();
}

void UNetWork::OnConnected()
{
}

void UNetWork::OnConnectionError(const FString& Error)
{
	FLib::Echo(TEXT("连接网络失败！原因：" + Error));
	FLib::Echo(TEXT("连接网络失败！原因：" + Error), false);
	Socket -> Connect();
}

void UNetWork::OnMessage(const FString& Source)
{
	TSharedPtr<FJsonObject> Data;
	if (!FJsonSerializer::Deserialize(TJsonReaderFactory<>::Create(Source), Data)) {
		FLib::Echo(TEXT("网络解析失败！"));
		return;
	}
	const int Status = Data -> GetIntegerField(FStatic::Status);
	const FString Message = Data -> GetStringField(FStatic::Message);
	if (Status == FStatic::Hundred) {
		return;
	}
	if (Status == FStatic::TwoHundredAndOne) {
		this -> Pong();
		return;
	}
	if (Status == FStatic::TwoHundredAndTwo) {
		FLib::Echo(TEXT("收到服务器推送的同步消息：") + FDateTime::Now() . ToString());
		const TSharedPtr<FJsonObject> Result = Data -> GetObjectField(FStatic::Result);
		// this -> Sync(Result);
		return;
	}
	if (Status == FStatic::TwoHundredAndThree) {
		const FString Result = Data -> GetStringField(FStatic::Result);
		// this -> GuestQuit(Result);
		return;
	}
}

void UNetWork::OnMessageSent(const FString& Source)
{
}

void UNetWork::OnClosed(int32 StatusCode, const FString& Reason, bool bWasClean)
{
	FLib::Echo(TEXT("与服务器的连接关闭！"), false);
	Socket -> Close();
}

void UNetWork::Pong()
{
}

void UNetWork::GuestQuit(const FString NickName)
{
}
