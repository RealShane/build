// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "Floor.generated.h"

USTRUCT(BlueprintType)
struct FBlockActor
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Side;
	
};

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

	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	UBoxComponent* LowSideBoxComponent;

	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	UBoxComponent* LeftSideBoxComponent;

	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	UBoxComponent* UpSideBoxComponent;

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

	//是否已放置
	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	bool IsSet = false;

	//是否附着
	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	bool IsAttach = false;

	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	FString BlockActorName;

	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	FString BlockActorSide;

	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	TArray<FBlockActor> BlockSideCache;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Right = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Low = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Left = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Up = false;

	void SetCollision(ECollisionEnabled::Type Type = ECollisionEnabled::NoCollision);

	void SetMaterial(FString Value);
	
	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
 
	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void RightOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
 
	UFUNCTION()
	void RightOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void LowOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
 
	UFUNCTION()
	void LowOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void LeftOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
 
	UFUNCTION()
	void LeftOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void UpOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
 
	UFUNCTION()
	void UpOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	bool Save(FString Name, FString CompName);
	bool Remove(FString Name, FString CompName);

};
