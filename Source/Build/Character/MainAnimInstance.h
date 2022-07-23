#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MainCharacter.h"
#include "MainAnimInstance.generated.h"

UCLASS()
class BUILD_API UMainAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	
	UMainAnimInstance();

	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BaseConfig")
	AMainCharacter* Main;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BaseConfig")
	float Direction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BaseConfig")
	float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BaseConfig")
	bool IsFalling = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BaseConfig")
	bool IsPunching = false;
	
};
