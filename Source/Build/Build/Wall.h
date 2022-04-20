#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Build/Lib/Static.h"
#include "Build/Lib/Lib.h"
#include "Build/Lib/Str.h"
#include "Struct/BlockActor.h"
#include "Wall.generated.h"

UCLASS()
class BUILD_API AWall : public AActor
{
	GENERATED_BODY()

public:
	AWall();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	UBoxComponent* BoxComponent;

	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	UBoxComponent* FrontBoxComponent;

	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	UBoxComponent* BackBoxComponent;

	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	UBoxComponent* LowSideBoxComponent;

	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	UBoxComponent* UpSideBoxComponent;


	//半长宽
	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	float HalfYZ;

	//高
	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	float HalfX;

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
	FString BlockFoundationName;

	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	FString BlockFoundationSide;

	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	FString BlockWallName;

	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	FString BlockWallSide;

	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	TMap<FString, FBlockActor> BlockSideCache;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Low = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Up = false;

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
	void LowOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
	                     class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                     const FHitResult& SweepResult);

	UFUNCTION()
	void LowOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
	                   class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void UpOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
	                    class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                    const FHitResult& SweepResult);

	UFUNCTION()
	void UpOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
	                  class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	bool Save(FString Name, FString CompName);
	bool Remove(FString Name, FString CompName);
};
