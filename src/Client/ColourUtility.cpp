#ifdef QOLMOD_IMGUI

#include "ColourUtility.h"

ImColor ColourUtility::GetColour(ClientColour colour)
{
	switch (colour)
	{
	case ClientColour::WindowBG:
		return ImColor(34, 40, 49);

	case ClientColour::Accent:
		return ImColor(115, 103, 210);

	case ClientColour::Text:
		return ImColor(255, 255, 255);

	case ClientColour::Hovered:
		return ImColor(68, 73, 83);

	case ClientColour::Pressed:
		return ImColor(32, 35, 39);
		
	case ClientColour::InputField:
		return ImColor(28, 28, 28);

	case ClientColour::InputCarot:
		return ImColor(120, 120, 120);

	case ClientColour::GradientLeft:
		return ImColor(125, 93, 211);

	case ClientColour::GradientRight:
		return ImColor(160, 87, 154);

	default:
		break;
	}

	return ImColor(255, 0, 255);
}

#endif