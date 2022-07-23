#include "Island.h"

AIsland::AIsland()
{
	PlayerControllerClass = AMainController::StaticClass();
	DefaultPawnClass = AMainCharacter::StaticClass();
	GameStateClass = AGlobal::StaticClass();
	PlayerNum = FStatic::Zero;
}

void AIsland::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
	for (TActorIterator<APlayerStart> It(GetWorld()); It; ++It) {
		StartArray . Emplace(*It);
		FLib::Echo("Find Player Start : " + (*It) -> GetName());
	}
}

FString AIsland::InitNewPlayer(APlayerController* NewPlayerController, const FUniqueNetIdRepl& UniqueId,
                                          const FString& Options, const FString& Portal)
{
	if (PlayerNum > FStatic::SeventyFive) {
		const FSoftObjectPath Path(*FStatic::StartMap);
		const TSoftObjectPtr<UWorld> Map(Path);
		UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(), Map, true);
		return Super::InitNewPlayer(NewPlayerController, UniqueId, Options, Portal);
	}
	NewPlayerController -> StartSpot = StartArray[FStatic::Zero];
	PlayerNum++;
	return Super::InitNewPlayer(NewPlayerController, UniqueId, Options, Portal);
}

void AIsland::BeginPlay()
{
	Super::BeginPlay();
	GEngine -> SetDynamicResolutionUserSetting(true);
}