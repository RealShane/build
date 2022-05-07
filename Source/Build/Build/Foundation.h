#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Build/Lib/Static.h"
#include "Build/Lib/Lib.h"
#include "Build/Lib/Str.h"
#include "Struct/BlockActor.h"
#include "Foundation.generated.h"

UCLASS()
class BUILD_API AFoundation : public AActor
{
	GENERATED_BODY()

public:
	AFoundation();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	UBoxComponent* BoxComponent;

	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	UBoxComponent* RangeBoxComponent;

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
	float HalfXY;

	//高
	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	float HalfZ;

	//模型与UE世界比例
	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	float XYZScale;

	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	bool IsBlock = false;

	//是否已放置
	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	bool IsSet = false;

	//是否附着
	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	bool IsAttach = false;

	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	int OverlapCount;

	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	int AttachCount;

	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	TMap<FString, FBlockActor> BlockSideCache;

	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	bool Right = false;

	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	bool Low = false;

	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	bool Left = false;

	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	bool Up = false;

	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	bool WallRight = false;

	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	bool WallLow = false;

	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	bool WallLeft = false;

	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	bool WallUp = false;

	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	float LandZ;

	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	FString UpRayName;

	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	FString DownRayName;

	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	FString RightDetectName;

	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	FString RightDetectCompName;

	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	FString LowDetectName;

	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	FString LowDetectCompName;

	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	FString LeftDetectName;

	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	FString LeftDetectCompName;

	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	FString UpDetectName;

	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	FString UpDetectCompName;

	void SetCollision(ECollisionEnabled::Type Type = ECollisionEnabled::NoCollision) const;

	void SetMaterial(FString Value) const;

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
	                    class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                    const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
	                  class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void RightOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
	                       class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                       const FHitResult& SweepResult);

	UFUNCTION()
	void RightOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
	                     class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void LowOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
	                     class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                     const FHitResult& SweepResult);

	UFUNCTION()
	void LowOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
	                   class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void LeftOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
	                      class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                      const FHitResult& SweepResult);

	UFUNCTION()
	void LeftOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
	                    class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void UpOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
	                    class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                    const FHitResult& SweepResult);

	UFUNCTION()
	void UpOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
	                  class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void RightDetectRay();
	void LowDetectRay();
	void LeftDetectRay();
	void UpDetectRay();
	bool Save(FString Name, FString CompName);
	bool Remove(FString Name, FString CompName);

	void LandHeight();
	float RayMax(bool IsUp);
};
