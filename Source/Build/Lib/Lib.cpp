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

FBlockActor Lib::SetSide(FString Key, FBlockActor Side, bool IsRemove)
{
	if (!IsRemove) {
		if (Key == "Right") {
			Side.Right = true;
		}
		if (Key == "Low") {
			Side.Low = true;
		}
		if (Key == "Left") {
			Side.Left = true;
		}
		if (Key == "Up") {
			Side.Up = true;
		}
	}else {
		if (Key == "Right") {
			Side.Right = false;
		}
		if (Key == "Low") {
			Side.Low = false;
		}
		if (Key == "Left") {
			Side.Left = false;
		}
		if (Key == "Up") {
			Side.Up = false;
		}
	}
	return Side;
}

FBuildCache Lib::SetSide(FString Key, FBuildCache Side, bool IsRemove, bool IsWall)
{
	if (!IsRemove) {
		if (IsWall) {
			if (Key == "Right") {
				Side.WallRight = true;
			}
			if (Key == "Low") {
				Side.WallLow = true;
			}
			if (Key == "Left") {
				Side.WallLeft = true;
			}
			if (Key == "Up") {
				Side.WallUp = true;
			}
		}else {
			if (Key == "Right") {
				Side.Right = true;
			}
			if (Key == "Low") {
				Side.Low = true;
			}
			if (Key == "Left") {
				Side.Left = true;
			}
			if (Key == "Up") {
				Side.Up = true;
			}
		}
	}else {
		if (IsWall) {
			if (Key == "Right") {
				Side.WallRight = false;
			}
			if (Key == "Low") {
				Side.WallLow = false;
			}
			if (Key == "Left") {
				Side.WallLeft = false;
			}
			if (Key == "Up") {
				Side.WallUp = false;
			}
		}else {
			if (Key == "Right") {
				Side.Right = false;
			}
			if (Key == "Low") {
				Side.Low = false;
			}
			if (Key == "Left") {
				Side.Left = false;
			}
			if (Key == "Up") {
				Side.Up = false;
			}
		}
	}
	return Side;
}

