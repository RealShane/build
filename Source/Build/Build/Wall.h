#pragma once

#include "CoreMinimal.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Build/GameMode/Global.h"
#include "Build/GameMode/Local.h"
#include "Struct/BlockActor.h"
#include "Build/Lib/Static.h"
#include "Build/Lib/Lib.h"
#include "Build/Lib/Str.h"
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

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(EditInstanceOnly, Replicated, Category = "BaseConfig")
	FString Key;

	//是否已放置
	UPROPERTY(EditInstanceOnly, Replicated, Category = "BaseConfig")
	bool IsSet = false;

	UPROPERTY(EditInstanceOnly, Replicated, Category = "BaseConfig")
	bool EnemyRange = false;

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
	bool Front = false;

	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	bool Back = false;

	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	bool Low = false;

	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	bool Up = false;

	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	float LandZ;

	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	FString UpRayName;

	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	FString DownRayName;

	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	FString LowDetectName;

	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	FString LowDetectCompName;

	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	FString UpDetectName;

	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	FString UpDetectCompName;

	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	ULocal* Local;

	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	AGlobal* Global;

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
	void FrontOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
	                       class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                       const FHitResult& SweepResult);

	UFUNCTION()
	void FrontOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
	                     class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void BackOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
	                      class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                      const FHitResult& SweepResult);

	UFUNCTION()
	void BackOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
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

	void LandHeight();
	float RayMax(bool IsUp);

	void SetOwnSide(FString Name, FString Side, bool IsRemove = false);
	void RangeDetectRay();
	UFUNCTION(Server, UnReliable)
	void JudgeEnemyRange(const FString& Index, int CID, int UID);
};
