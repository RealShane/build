#include "NetWork.h"

UNetWork::UNetWork()
{
	Socket = FWebSocketsModule::Get() . CreateWebSocket(FStatic::WSURL, FStatic::WS);
	Socket -> OnConnected() . AddUObject(this, &UNetWork::OnConnected);
	Socket -> OnConnectionError() . AddUObject(this, &UNetWork::OnConnectionError);
	Socket -> OnMessage() . AddUObject(this, &UNetWork::OnMessage);
	Socket -> OnMessageSent() . AddUObject(this, &UNetWork::OnMessageSent);
	Socket -> OnClosed() . AddUObject(this, &UNetWork::OnClosed);
	Socket -> Connect();
}

void UNetWork::BeginPlay()
{
	Super::BeginPlay();
}

void UNetWork::Init(UWorld* Main)
{
	this -> World = Main;
}

void UNetWork::OnConnected()
{
	
}

void UNetWork::OnConnectionError(const FString& Error)
{
	FLib::Echo(TEXT("连接网络失败！原因：" + Error));
}

void UNetWork::OnMessage(const FString& Source)
{
	FLib::Echo(Source, true);
	TSharedPtr<FJsonObject> Data;
	if (!FJsonSerializer::Deserialize(TJsonReaderFactory<>::Create(Source), Data)) {
		FLib::Echo(TEXT("网络解析失败！"));
		return;
	}
	const int Status = Data -> GetIntegerField(FStatic::Status);
	const FString Message = Data -> GetStringField(FStatic::Message);
	if (Status == FStatic::Hundred) {
		FLib::Echo(Message);
		return;
	}
	if (Status == FStatic::TwoHundredAndOne) {
		FLib::Echo(TEXT("已连接网络！"));
		return;
	}
	if (Status == FStatic::TwoHundredAndTwo) {
		const TSharedPtr<FJsonObject> Result = Data -> GetObjectField(FStatic::Result);
		this -> FreshJoin(Result);
		return;
	}
	if (Status == FStatic::TwoHundredAndThree) {
		const TSharedPtr<FJsonObject> Result = Data -> GetObjectField(FStatic::Result);
		this -> Move(Result);
		return;
	}
	
}

void UNetWork::OnMessageSent(const FString& Source)
{
	
}

void UNetWork::OnClosed(int32 StatusCode, const FString& Reason, bool bWasClean)
{
	FLib::Echo(TEXT("与服务器的连接关闭！"));
}

void UNetWork::Send(FString String)
{
	FLib::Echo("send : " + String, true);
	Socket -> Send(String);
}

void UNetWork::FreshJoin(const TSharedPtr<FJsonObject> Data)
{
	const FVector Location = FVector(
		Data -> GetNumberField(FStatic::LocationX),
		Data -> GetNumberField(FStatic::LocationY),
		Data -> GetNumberField(FStatic::LocationZ)
	);
	const FRotator Rotation = FRotator(
		Data -> GetNumberField(FStatic::RotationPitch),
		Data -> GetNumberField(FStatic::RotationYaw),
		Data -> GetNumberField(FStatic::RotationRoll)
	);
	ACloneCharacter* Guest = World -> SpawnActor<ACloneCharacter>(Location, Rotation);
	FString Name = FStatic::Guest + FStatic::Underline + Data -> GetStringField(FStatic::Name);
	Guest -> SetActorLabel(Name);
	Players . Emplace(Name, Guest);
}

void UNetWork::Move(const TSharedPtr<FJsonObject> Data)
{
	const FVector Location = FVector(
		Data -> GetNumberField(FStatic::LocationX),
		Data -> GetNumberField(FStatic::LocationY),
		Data -> GetNumberField(FStatic::LocationZ)
	);
	const FRotator Rotation = FRotator(
		Data -> GetNumberField(FStatic::RotationPitch),
		Data -> GetNumberField(FStatic::RotationYaw),
		Data -> GetNumberField(FStatic::RotationRoll)
	);
	const FString Name = FStatic::Guest + FStatic::Underline + Data -> GetStringField(FStatic::Name);
	Players[Name] -> SetActorLocation(Location);
	Players[Name] -> SetActorRotation(Rotation);
}
