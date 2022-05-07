#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Build/Lib/Static.h"
#include "Build/Lib/Lib.h"
#include "CloneCharacter.generated.h"

UCLASS()
class BUILD_API ACloneCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ACloneCharacter();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//是否移动
	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	bool IsMoving = false;

	//骨骼模型
	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	USkeletalMesh* SkeletalMesh;

	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	UAnimSequence* AnimSequence;

	void AnimPlay(FString Value, bool loop = false);

	FString GetPlayingAnimName() const;

	void Construct();
	void CreateModel() const;
};
