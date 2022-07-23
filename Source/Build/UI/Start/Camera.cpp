#include "Camera.h"

#include "Build/UI/Notify/PopNotice.h"

ACamera::ACamera()
{
	PrimaryActorTick . bCanEverTick = false;
	Request = CreateDefaultSubobject<URequest>(*FStatic::Request);
	UI = CreateDefaultSubobject<UUIFacade>(*FStatic::UI);
	AutoPossessPlayer = EAutoReceiveInput::Player0;
	Request -> WidgetSwitch . AddUObject(this, &ThisClass::SwitchToServerList);
}

void ACamera::BeginPlay()
{
	Super::BeginPlay();
	Local = GetGameInstance() -> GetSubsystem<ULocal>();
	Login = UI -> GetInstance<ULogin>(*FStatic::LoginBP);
	Menu = UI -> GetInstance<UMenu>(*FStatic::MenuBP);
	Menu -> Pop . AddDynamic(this, &ThisClass::Pop);
	PopLayer = UI -> GetInstance<UPopLayer>(*FStatic::PopLayerBP);
	if (Local -> LostConnection) {
		Request -> Server();
		Request -> SetLocal(Local);
	} else {
		Login -> Open(Request);
	}
}

void ACamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACamera::SwitchToServerList()
{
	if (!Local -> LostConnection) {
		Login -> Close();
	}
	Menu -> Open();
}

void ACamera::Pop(FString Message, FString Color)
{
	PopLayer -> Pop(Message, Color);
}
