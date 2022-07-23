#include "MainAnimInstance.h"

#include "Animation/AnimNode_StateMachine.h"

UMainAnimInstance::UMainAnimInstance()
{
	Speed = FStatic::Zero;
	Direction = FStatic::Zero;
}

void UMainAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	Main = Cast<AMainCharacter>(GetOwningActor());
}

void UMainAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (Main == nullptr) {
		return;
	}
	const int MachineIndex = GetStateMachineIndex(*FStatic::BehaviorState);
	const FAnimNode_StateMachine* MachineInstance = GetStateMachineInstance(MachineIndex);
	const int StateIndex = MachineInstance -> GetCurrentState();
	const bool IsPunch = MachineInstance -> GetStateInfo(StateIndex) . StateName . ToString() == FStatic::Punch;
	const bool IsRunPunch = MachineInstance -> GetStateInfo(StateIndex) . StateName . ToString() == FStatic::RunPunch;
	if (IsPunch || IsRunPunch) {
		if (GetRelevantAnimTimeRemaining(MachineIndex, StateIndex) < FStatic::ZeroPointOne) {
			Main -> Punch = false;
		}
	}
	IsFalling = Main -> GetCharacterMovement() -> IsFalling();
	IsPunching = Main -> Punch;
	Speed = Main -> Speed;
	Direction = CalculateDirection(Main -> GetVelocity(), Main -> GetActorRotation());
}
