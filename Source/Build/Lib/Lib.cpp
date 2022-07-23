#include "Lib.h"

void FLib::Echo(const float Value, const bool IsGoLog, const int Time)
{
	const FString Message = FString::Printf(TEXT("%f"), Value);
	if (IsGoLog) {
		UE_LOG(LogTemp, Warning, TEXT("%s"), *Message);
	} else {
		GEngine -> AddOnScreenDebugMessage(-1, Time, FColor::Green, Message);
	}
}

void FLib::Echo(const FString Value, const bool IsGoLog, const int Time)
{
	const FString Message = FString::Printf(TEXT("%s"), *Value);
	if (IsGoLog) {
		UE_LOG(LogTemp, Warning, TEXT("%s"), *Message);
	} else {
		GEngine -> AddOnScreenDebugMessage(-1, Time, FColor::Green, Message);
	}
}

float FLib::Max(const float Num1, const float Num2)
{
	return Num1 > Num2 ? Num1 : Num2;
}

FString FLib::Salt(const int Bit)
{
	const FString Chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
	FString Salt;
	for (int Index = FStatic::Zero; Index < Bit; Index++) {
		const int RandomIndex = FMath::RandRange(FStatic::Zero, Chars.Len() - FStatic::One);
		Salt += Chars[RandomIndex];
	}
	return Salt;
}
