#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/PointLightComponent.h"
#include "Torch.generated.h"

UCLASS()
class BUILD_API ATorch : public AActor
{
	GENERATED_BODY()

public:
	ATorch();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	UParticleSystemComponent* ParticleSystemComponent;

	UPROPERTY(EditInstanceOnly, Category = "BaseConfig")
	UPointLightComponent* PointLightComponent;

	void LightOn() const;
};
