#pragma once

#include "CoreMinimal.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"
#include "GameFramework/GameModeBase.h"
#include "Build/Character/MainController.h"
#include "Build/Character/MainCharacter.h"
#include "Island.generated.h"

UCLASS()
class BUILD_API AIsland : public AGameModeBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	AIsland();

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

	virtual FString InitNewPlayer(APlayerController* NewPlayerController, const FUniqueNetIdRepl& UniqueId, const FString& Options, const FString& Portal) override;

	int PlayerNum;

	TArray<APlayerStart*> StartArray;
};
