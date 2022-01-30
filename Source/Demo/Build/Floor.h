// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "Floor.generated.h"

UCLASS()
class DEMO_API AFloor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFloor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	UBoxComponent* BoxComponent;

	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	UBoxComponent* RightSideBoxComponent;

	//半长宽
	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	float HalfXY = 200;

	//高
	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	float HalfZ = 30;

	//模型与UE世界比例
	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	float XYZScale = 50;

	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	bool IsBlock = false;

	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	FString BlockActorName;

	void SetCollision(ECollisionEnabled::Type Type = ECollisionEnabled::NoCollision);

	void SetMaterial(FString Value);
	
	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
 
	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};
