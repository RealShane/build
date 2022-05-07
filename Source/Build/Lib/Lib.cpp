#include "Lib.h"

void FLib::Echo(const float Value, const bool IsGoLog)
{
	const FString Message = FString::Printf(TEXT("%f"), Value);
	if (IsGoLog) {
		UE_LOG(LogTemp, Warning, TEXT("%s"), *Message);
	} else {
		GEngine -> AddOnScreenDebugMessage(-FStatic::One, FStatic::One, FColor::Green, Message);
	}
}

void FLib::Echo(const FString Value, const bool IsGoLog)
{
	const FString Message = FString::Printf(TEXT("%s"), *Value);
	if (IsGoLog) {
		UE_LOG(LogTemp, Warning, TEXT("%s"), *Message);
	} else {
		GEngine -> AddOnScreenDebugMessage(-FStatic::One, FStatic::One, FColor::Green, Message);
	}
}

float FLib::Max(const float Num1, const float Num2)
{
	return Num1 > Num2 ? Num1 : Num2;
}
