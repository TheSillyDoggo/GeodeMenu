#include "Utils.hpp"
#include <Geode/Geode.hpp>
#include <imgui-cocos.hpp>

using namespace geode::prelude;

float roundUpToMultipleOf2(float num)
{
	float roundedNum = std::round(num / 2.0f) * 2.0f;
	return roundedNum;
}

float scaleFloat(float v, float min, float max)
{
	return (max - min) * v + min;
}

float unscaleFloat(float v, float min, float max)
{
	float minValue = min;
	float maxValue = max;
	float originalValue = (v - minValue) / (maxValue - minValue);
	return originalValue;
}

geode::Patch* createPatchSafe(void *address, const geode::ByteVector &data)
{
	auto patch = geode::Mod::get()->patch(address, data);

	if (patch.has_error())
		geode::log::error("Failed to apply patch: {}, data: {}, error: {}", address, data, patch.err());

	return patch.unwrapOr(nullptr);
}

cocos2d::CCRect getScreenSafeArea()
{
	cocos2d::CCRect insets;

	#ifdef GEODE_IS_IOS
	insets = getScreenSafeAreaiOS();
	#endif

	return insets;
}

void ImGuiExt::colouredText(std::string label)
{
	bool isCol = false;
	bool v = false;
	int c = 0;
	ImColor colour = ImColor(1.0f, 1.0f, 1.0f);

	for (auto ch : label)
	{
		bool newLine = ch == '\n';

		if (isCol)
		{
			if (c == 0 && ch == '/')
			{
				colour = ImColor(1.0f, 1.0f, 1.0f);
				v = true;
			}

			if (c == 1 && !v)
			{
				if (ch == 'a')
					colour = ImColor(150, 50, 255, 255);

				if (ch == 'b')
					colour = ImColor(74, 82, 225, 255);

				if (ch == 'c')
					colour = ImColor(255, 255, 150, 255);

				if (ch == 'd')
					colour = ImColor(255, 150, 255, 255);

				if (ch == 'f')
					colour = ImColor(150, 255, 255, 255);

				if (ch == 'g')
					colour = ImColor(64, 227, 72, 255);

				if (ch == 'j')
					colour = ImColor(50, 200, 255, 255);

				if (ch == 'l')
					colour = ImColor(96, 171, 239, 255);

				if (ch == 'o')
					colour = ImColor(255, 165, 75, 255);

				if (ch == 'p')
					colour = ImColor(255, 0, 255, 255);

				if (ch == 'r')
					colour = ImColor(255, 90, 90, 255);

				if (ch == 's')
					colour = ImColor(255, 220, 65, 255);

				if (ch == 'y')
					colour = ImColor(255, 255, 0, 255);
			}

			if (c == 2)
			{
				isCol = false;
				v = false;
			}

			c++;

			continue;
		}

		if (ch == '<')
		{
			isCol = true;
			v = false;
			c = 0;

			continue;
		}

		ImGui::TextColored(colour.Value, "%s", fmt::format("{}", ch).c_str());

		if (!newLine)
			ImGui::SameLine();
	}
}