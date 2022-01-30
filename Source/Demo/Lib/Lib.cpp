// Fill out your copyright notice in the Description page of Project Settings.


#include "Lib.h"

void Lib::echo(float Value, bool IsGoLog)
{
	FString Message = FString::Printf(TEXT("%f"),  Value);
	if (IsGoLog){
		UE_LOG(LogTemp, Warning, TEXT("%s"), *Message);
	}else{
		GEngine -> AddOnScreenDebugMessage(-1, 1.f, FColor::Green, Message);
	}
}

void Lib::echo(FString Value, bool IsGoLog)
{
	FString Message = FString::Printf(TEXT("%s"),  *Value);
	if (IsGoLog){
		UE_LOG(LogTemp, Warning, TEXT("%s"), *Message);
	}else{
		GEngine -> AddOnScreenDebugMessage(-1, 1.f, FColor::Green, Message);
	}
	
}
