// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "Person.generated.h"



UCLASS()
class DEMO_API APerson : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APerson();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:

	//静态模型组件
	UPROPERTY(VisibleInstanceOnly, Category = "BaseConfig")
	UStaticMeshComponent* StaticMeshComponent;

	//骨骼模型组件
	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	USkeletalMeshComponent* SkeletalMeshComponent;

	//骨骼模型
	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	USkeletalMesh* SkeletalMesh;

	//摄像机摇臂
	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	USpringArmComponent* SpringArmComponent;
	
	//摄像机
	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	UCameraComponent* CameraComponent;

	//胶囊体组件
	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	UCapsuleComponent* CapsuleComponent;

private:
	void Construct();
	void CreateModel();
	void CreateCamera();
	UFUNCTION()
	void OnHit(class UPrimitiveComponent* OverLapComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);
};
