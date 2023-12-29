#include "ColourUtility.h"

ImColor ColourUtility::GetColour(ClientColour colour)
{
	switch (colour)
	{
	case ClientColour::WindowBG:
		return ImColor(34, 40, 49);

	case ClientColour::Accent:
		return ImColor(113, 96, 232);

	case ClientColour::Text:
		return ImColor(255, 255, 255);

	case ClientColour::Hovered:
		return ImColor(54, 60, 68);

	case ClientColour::Pressed:
		return ImColor(32, 35, 39);
		
	case ClientColour::InputField:
		return ImColor(28, 28, 28);

	case ClientColour::InputCarot:
		return ImColor(120, 120, 120);

	default:
		break;
	}

	return ImColor(255, 0, 255);
}