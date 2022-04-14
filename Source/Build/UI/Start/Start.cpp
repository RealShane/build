#include "Start.h"

void UStart::Layout()
{
	Button = WidgetTree -> ConstructWidget<UButton>(UButton::StaticClass(), TEXT("Button1"));
}
