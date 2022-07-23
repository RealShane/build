#include "Global.h"

void AGlobal::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	// DOREPLIFETIME(ThisClass, Joint);
	// DOREPLIFETIME(ThisClass, Buildings);
}
