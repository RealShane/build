#include "Lang.h"

FLang::FLang()
{
	Choice = ZhCn;
	Load(Choice);
}

FString FLang::Get(const FString Value) const
{
	return Data -> GetStringField(Value);
}

void FLang::Load(const FString Language)
{
	FString Json;
	if (!FFileHelper::LoadFileToString(Json, *Language)) {
		FLib::Echo(TEXT("加载语言失败"), true);
		return;
	}
	const TSharedRef<TJsonReader<TCHAR>> Reader = TJsonReaderFactory<TCHAR>::Create(Json);
	if (!FJsonSerializer::Deserialize(Reader, Data)) {
		FLib::Echo(TEXT("解析语言失败"), true);
	}
}
