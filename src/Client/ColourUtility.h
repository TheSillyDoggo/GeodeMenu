#include <imgui-cocos.hpp>

class ColourUtility
{
	public:
		
		enum ClientColour
		{
			WindowBG,
			Accent,
			Text,
			Hovered,
			Pressed,
			InputField,
			InputCarot
		};

		static ImColor GetColour(ClientColour colour);
};